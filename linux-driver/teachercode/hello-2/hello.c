#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");

#define HELLO_MAJOR 250
#define HELLO_MINOR 0
#define DEVICE_NUMBER 1

static struct cdev cdev;

static struct file_operations hello_fops = {
	.owner = THIS_MODULE,
};

static int hello_init(void)
{
	int ret = 0;
	dev_t devno = MKDEV(HELLO_MAJOR, HELLO_MINOR);

	ret = register_chrdev_region(devno, DEVICE_NUMBER, "hello");
	if (ret < 0) {
		printk("failed: register_chrdev_region\n");
		return ret;
	}

	cdev_init(&cdev, &hello_fops);
	cdev.owner = THIS_MODULE;
	ret = cdev_add(&cdev, devno, DEVICE_NUMBER);
	if (ret < 0) {
		printk("faield: cdev_add\n");
		return ret;
	}

	printk("hello world\n");
	return 0;
}

static void hello_exit(void)
{
	dev_t devno = MKDEV(HELLO_MAJOR, HELLO_MINOR);
	cdev_del(&cdev);
	unregister_chrdev_region(devno, DEVICE_NUMBER);
	printk("goodbye world\n");
}

module_init(hello_init);
module_exit(hello_exit);
