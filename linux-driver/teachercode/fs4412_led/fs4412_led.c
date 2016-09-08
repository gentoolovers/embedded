#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <asm/io.h>
#include <asm/uaccess.h>

#include "fs4412_led.h"

MODULE_LICENSE("GPL");

#define LED_MAJOR 250
#define LED_MINOR 0

#define GPX1_BASE	0x11000C20
#define GPX2_BASE	0x11000C40
#define GPF3_BASE	0x114001E0

#define CONFOFF 0x00
#define DATAOFF 0x04

struct fs4412_led
{
	void __iomem *gpx1_base;
	void __iomem *gpx2_base;
	void __iomem *gpf3_base;
	struct cdev cdev;
};

static struct fs4412_led *led;

static void led_off_all(void)
{
	writel(readl(led->gpx2_base + DATAOFF) & ~(0x1 << 7), led->gpx2_base + DATAOFF);
	writel(readl(led->gpx1_base + DATAOFF) & ~(0x1 << 0), led->gpx1_base + DATAOFF);
	writel(readl(led->gpf3_base + DATAOFF) & ~(0x3 << 4), led->gpf3_base + DATAOFF);
}

static void fs4412_led_off(int nr)
{
	switch(nr) {
	case LED1:
		writel(readl(led->gpx2_base + DATAOFF) & ~(0x1 << 7), led->gpx2_base + DATAOFF);
		break;
	case LED2:
		writel(readl(led->gpx1_base + DATAOFF) & ~(0x1 << 0), led->gpx1_base + DATAOFF);
		break;
	case LED3:
		writel(readl(led->gpf3_base + DATAOFF) & ~(0x1 << 4), led->gpf3_base + DATAOFF);
		break;
	case LED4:
		writel(readl(led->gpf3_base + DATAOFF) & ~(0x1 << 5), led->gpf3_base + DATAOFF);
		break;
	}
}

static void fs4412_led_on(int nr)
{
	switch(nr) {
	case LED1:
		writel(readl(led->gpx2_base + DATAOFF) | (0x1 << 7), led->gpx2_base + DATAOFF);
		break;
	case LED2:
		writel(readl(led->gpx1_base + DATAOFF) | (0x1 << 0), led->gpx1_base + DATAOFF);
		break;
	case LED3:
		writel(readl(led->gpf3_base + DATAOFF) | (0x1 << 4), led->gpf3_base + DATAOFF);
		break;
	case LED4:
		writel(readl(led->gpf3_base + DATAOFF) | (0x1 << 5), led->gpf3_base + DATAOFF);
		break;
	}
}
	
static int fs4412_led_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int fs4412_led_release(struct inode *inode, struct file *file)
{	
	led_off_all();

	return 0;
}
	
static long fs4412_led_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int nr;

	if (copy_from_user(&nr, (void *)arg, sizeof(int)))
		return -EFAULT;

	switch(cmd) {
		case LED_ON:
			fs4412_led_on(nr);
			break;
		case LED_OFF:
			fs4412_led_off(nr);
			break;
		default:
			return -EINVAL;
	}

	return 0;
}
	
static ssize_t fs4412_led_write(struct file *file, const char __user *buf, size_t count, loff_t *loff)
{
	struct led_action action;

	if (copy_from_user(&action, buf, sizeof(struct led_action)))
		return -EFAULT;
	
	switch(action.act) {
		case LED_ON:
			fs4412_led_on(action.nr);
			break;
		case LED_OFF:
			fs4412_led_off(action.nr);
			break;
		default:
			return -EINVAL;
	}

	return count;
}

static struct file_operations fs4412_led_fops = {
	.owner = THIS_MODULE,
	.open = fs4412_led_open,
	.release = fs4412_led_release,
	.unlocked_ioctl = fs4412_led_unlocked_ioctl,
	.write = fs4412_led_write
};

static int fs4412_led_init(void)
{

	int ret;
	dev_t devno = MKDEV(LED_MAJOR, LED_MINOR);

	led = kmalloc(sizeof(*led), GFP_KERNEL);
	if (led == NULL) {
		printk("failed: kmalloc\n");
		return -ENOMEM;
	}
	memset(led, 0, sizeof(*led));

	ret = register_chrdev_region(devno, 1, "led");
	if (ret < 0) {
		printk("failed: register_chrdev_region\n");	
		goto err1;
	}

	cdev_init(&led->cdev, &fs4412_led_fops);
	led->cdev.owner = THIS_MODULE;
	ret = cdev_add(&led->cdev, devno, 1);
	if (ret < 0) {
		printk("failed: cdev_add\n");
		goto err2;
	}

	led->gpx1_base = ioremap(GPX1_BASE, 0x8);
	if (led->gpx1_base == NULL) {
		ret = -ENOMEM;
		printk("failed: ioremap gpx1_base\n");
		goto err3;
	}

	led->gpx2_base = ioremap(GPX2_BASE, 0x8);
	if (led->gpx2_base == NULL) {
		ret = -ENOMEM;
		printk("failed: ioremap gpx2_base\n");
		goto err4;
	}

	led->gpf3_base = ioremap(GPF3_BASE, 0x8);
	if (led->gpf3_base == NULL) {
		ret = -ENOMEM;
		printk("failed: ioremap gpf3_base\n");
		goto err5;
	}

	writel((readl(led->gpx2_base + CONFOFF) & ~(0xf << 28)) | (0x1 << 28), led->gpx2_base + CONFOFF);
	writel((readl(led->gpx1_base + CONFOFF) & ~(0xf << 0)) | (0x1 << 0), led->gpx1_base + CONFOFF);
	writel((readl(led->gpf3_base + CONFOFF) & ~(0xff << 16)) | (0x11 << 16), led->gpf3_base + CONFOFF);


	led_off_all();

	return 0;


err5:
	iounmap(led->gpx2_base);
err4:
	iounmap(led->gpx1_base);
err3:
	cdev_del(&led->cdev);
err2:
	unregister_chrdev_region(devno, 1);
err1:
	kfree(led);	
	return ret;
};

static void fs4412_led_exit(void)
{
	dev_t devno = MKDEV(LED_MAJOR, LED_MINOR);
	iounmap(led->gpx1_base);
	iounmap(led->gpx2_base);
	iounmap(led->gpf3_base);
	cdev_del(&led->cdev);
	unregister_chrdev_region(devno, 1);
	kfree(led);	
}

module_init(fs4412_led_init);
module_exit(fs4412_led_exit);
