#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <asm/current.h>
#include <linux/sched.h>

#include <linux/cdev.h>
#include <linux/fs.h>

#include <linux/platform_device.h>
#include <linux/device.h>

#include <linux/interrupt.h>

static struct cdev platformcdev;

static int major = 0;
static int minor = 0;
static int count = 1;

static unsigned int irq;

#define DEVNAME "fskey"
static struct class *cls = NULL;

irqreturn_t key_handler(int irq, void *arg)
{
	printk("%s:%d\n", __func__, __LINE__);

	return IRQ_HANDLED;
}

static int fskey_open(struct inode *inode, struct file *filp)
{
	int ret;

	printk("%s:%d, pid = %d\n", __func__, __LINE__, current->pid);

	ret = request_irq(irq, key_handler, IRQF_TRIGGER_FALLING, DEVNAME, NULL);
	if(0 > ret){
		return ret;
	}

	return 0;
}

static int fskey_release(struct inode *inode, struct file *filp)
{
	printk("%s:%d, pid = %d\n", __func__, __LINE__, current->pid);
	free_irq(irq, NULL);
	return 0;
}

static struct file_operations fops = {
	.owner	= THIS_MODULE,
	.open	= fskey_open,
	.release= fskey_release,
};

static int fskeydrv_probe(struct platform_device *pdev)
{
	int ret, i;
	dev_t dev;
	struct device *device = NULL;

	irq = platform_get_irq(pdev, 0);
	if(0 > irq){
		return irq;
	}
	printk("IRQ: %d\n", irq);

	printk("%s:%d, pid = %d\n", __func__, __LINE__, current->pid);
	cdev_init(&platformcdev, &fops);

	ret = alloc_chrdev_region(&dev, minor, count, DEVNAME);
	if(ret){
		return ret;
	}
	major = MAJOR(dev);
	
	ret = cdev_add(&platformcdev, dev, count);
	if(ret){
		goto ERR_STEP;
	}

	cls = class_create(THIS_MODULE, DEVNAME);
	if (IS_ERR(cls)) {
		ret = PTR_ERR(cls);
		goto ERR_STEP1;
	}

	for(i = minor; i < count+minor; i++){
		device = device_create(cls, NULL, MKDEV(major, i), NULL, "%s%d", DEVNAME, i);
		if (IS_ERR(device)) {
			ret = PTR_ERR(device);
			goto ERR_STEP2;
		}		
	}

	return 0;

ERR_STEP2:
	for(i--; i >= minor; i--){
		device_destroy(cls, MKDEV(major, i));
	}

	class_destroy(cls);

ERR_STEP1:
	cdev_del(&platformcdev);

ERR_STEP:
	unregister_chrdev_region(dev, count);
	return ret;
}	

static int fskeydrv_remove(struct platform_device *pdev)
{
	int i;
	printk("%s:%d, pid = %d\n", __func__, __LINE__, current->pid);

	//1. free resorce
	unregister_chrdev_region(MKDEV(major, minor), count);

	for(i = minor; i < count+minor; i++){
		device_destroy(cls, MKDEV(major, i));
	}
	class_destroy(cls);

	//2.unregister
	cdev_del(&platformcdev);
	return 0;
}

static struct of_device_id idtbl[] = {
	{  .compatible = "Farsight,fskey", },
	{  /**/ },
};
MODULE_DEVICE_TABLE(of, idtbl);

static struct platform_driver fskeydrv = {
	.driver = {
		.name = "fskey",
		.of_match_table = idtbl,
	},

	.probe = fskeydrv_probe,
	.remove= fskeydrv_remove,
};

module_platform_driver(fskeydrv);

MODULE_LICENSE("GPL");
