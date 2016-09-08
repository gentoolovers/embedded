#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <asm/uaccess.h>

#include "hello.h"

MODULE_LICENSE("GPL");

#define HELLO_MAJOR 250
#define HELLO_MINOR 0
#define DEVICE_NUMBER 1

static struct cdev cdev;
static char data[1024];
	
static int hello_open(struct inode *inode, struct file *file)
{
	printk("hello open success!\n");
	return 0;
}

static int hello_close(struct inode *inode, struct file *file)
{
	printk("hello close success!\n");
	return 0;
}
	
static ssize_t hello_write(struct file *file, const char __user *buf, size_t count, loff_t *loff)
{
	if (count < 0)
		return -EINVAL;
	if (count > 1023)
		count = 1023;

	if (copy_from_user(data, buf, count))
		return -EFAULT;

	/*
	 * 将数据写入设备
	 */
	data[count] = '\0';

	printk("data = %s\n", data);

	return count;
}

static ssize_t hello_read(struct file *file, char __user *buf, size_t count, loff_t *loff)
{
	if (count < 0)
		return -EINVAL;
	if (count > 1023)
		count = 1023;

	/*
	 * 从设备中读取数据
	 */

	if (copy_to_user(buf, data, count))
		return -EFAULT;

	return count;
}

	
static long hello_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int data;

	if (_IOC_TYPE(cmd) != 'H')
		return -EINVAL;

	if (_IOC_NR(cmd) > 1)
		return -EINVAL;

	switch(cmd) {
	case HELLO_CMD1:
		printk("hello cmd1\n");
		break;
	case HELLO_CMD2:
		printk("hello cmd2\n");
		if (copy_from_user(&data, (void *)arg, _IOC_SIZE(cmd)))
			return -EFAULT;

		printk("data = %d\n", data);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static struct file_operations hello_fops = {
	.owner 		= THIS_MODULE,
	.open 		= hello_open,
	.write		= hello_write,
	.release 	= hello_close,
	.read 		= hello_read,
	.unlocked_ioctl = hello_unlocked_ioctl,
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
