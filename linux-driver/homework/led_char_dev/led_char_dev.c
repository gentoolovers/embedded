
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <asm/io.h>
#include <asm/uaccess.h>

#include "fs4412_led.h"

MODULE_LICENSE("GPL");


static int  led_chardev_major=LED_MAJOR;

struct chrcdev
{
    struct cdev cdev;
    void __iomem* gpx1_base;
    void __iomem* gpx2_base;
    void __iomem* gpf3_base;
};

struct chrcdev* led_chrcdev;

static int led_chardev_open(struct inode *inode, struct file *filp)
{
    return 0;
}
static int led_chardev_release(struct inode* inode,struct file* file)
{

    return 0;
}
static void set_fs4412_led_on(int nr)
{
    switch(nr)
    {
    case LED_2:
        writel(readl(led_chrcdev->gpx2_base + DATAOFFSET) | (0x1 << 7), led_chrcdev->gpx2_base + DATAOFFSET);
        break;
    case LED_3:
        writel(readl(led_chrcdev->gpx1_base + DATAOFFSET) | (0x1 << 0), led_chrcdev->gpx1_base + DATAOFFSET);
        break;
    case LED_4:
        writel(readl(led_chrcdev->gpf3_base + DATAOFFSET) | (0x1 << 4), led_chrcdev->gpf3_base + DATAOFFSET);
        break;
    case LED_5:
        writel(readl(led_chrcdev->gpf3_base + DATAOFFSET) | (0x1 << 5), led_chrcdev->gpf3_base + DATAOFFSET);
        break;
    default:
        break;
    }
}
static void set_fs4412_led_off(int nr)
{
    switch(nr)
    {
    case LED_2:
        writel(readl(led_chrcdev->gpx2_base + DATAOFFSET) & ~(0x1 << 7), led_chrcdev->gpx2_base + DATAOFFSET);
        break;
    case LED_3:
        writel(readl(led_chrcdev->gpx1_base + DATAOFFSET) & ~(0x1 << 0), led_chrcdev->gpx1_base + DATAOFFSET);
        break;
    case LED_4:
        writel(readl(led_chrcdev->gpf3_base + DATAOFFSET) & ~(0x1 << 4), led_chrcdev->gpf3_base + DATAOFFSET);
        break;
    case LED_5:
        writel(readl(led_chrcdev->gpf3_base + DATAOFFSET) & ~(0x1 << 5), led_chrcdev->gpf3_base + DATAOFFSET);
        break;
    default:
        break;
    }
}


static long led_chardev_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

    int nr;

    if (_IOC_TYPE(cmd) !='L')
        return -EINVAL;
    if (_IOC_NR(cmd) > 1)
        return -EINVAL;
    if(copy_from_user(&nr,(void*)arg,_IOC_SIZE(cmd)))
        return -EINVAL;

    switch(cmd)
    {

    case LED_ON:
        set_fs4412_led_on(nr);
            break;
    case LED_OFF:
        set_fs4412_led_off(nr);

        break;
    default:
        return -EINVAL;
    }
    return 0;
}

static struct file_operations led_chardev_fops={
    .owner  =  THIS_MODULE,
    .open   =  led_chardev_open,
    .unlocked_ioctl = led_chardev_unlocked_ioctl,
    .release = led_chardev_release,
};
static __init int led_chardev_init(void)
{
    int ret=0;
    dev_t devno=MKDEV(led_chardev_major,LED2_MINOR);
    led_chrcdev = kmalloc(sizeof(struct chrcdev),GFP_KERNEL);
    if(!led_chrcdev)
    {
        ret=-ENOMEM;
        return ret;
    }
    if(led_chardev_major)
    {
        ret = register_chrdev_region(devno,DEVICE_NUMBER,"fs4412_led");
        if(ret <0)
        {
            printk("failed:register_chrdev_region\n");
            goto err1;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&devno,0,DEVICE_NUMBER,"fs4412_led");
        if(ret <0)
        {
            printk("failed:alloc_chrdev_region\n");
            goto err1;
        }
        led_chardev_major = MAJOR(devno);
    }
    cdev_init(&(led_chrcdev->cdev),&led_chardev_fops);
    led_chrcdev->cdev.owner=THIS_MODULE;
    ret=cdev_add(&(led_chrcdev->cdev),devno,DEVICE_NUMBER);
    if(ret <0)
    {
        printk("failed:cdev_add\n");
        goto err2;
    }
    led_chrcdev->gpx1_base=ioremap(GPX1_BASE,0x8);
    if(led_chrcdev->gpx1_base == NULL)
    {
        ret= -ENOMEM;
        printk("failed:ioremap gpx1_base\n");
        goto err3;
    }
    led_chrcdev->gpx2_base=ioremap(GPX2_BASE,0x8);
    if(led_chrcdev->gpx2_base == NULL)
    {
        ret = -ENOMEM;
        printk("failed:ioremap gpx2_base\n");
        goto err4;
    }
    led_chrcdev->gpf3_base =ioremap(GPF3_BASE,0x8);
    if(led_chrcdev->gpf3_base == NULL)
    {
        ret=-ENOMEM;
        printk("failed:ioremap gpf3_base\n");
        goto err5;
    }

    writel((readl(led_chrcdev->gpx2_base + CONFOFFSET) & ~(0xf<< 28)) | (0x1 << 28),led_chrcdev->gpx2_base + CONFOFFSET);

    writel((readl(led_chrcdev->gpx1_base + CONFOFFSET) & ~(0xf<< 0)) | (0x1 << 0),led_chrcdev->gpx1_base + CONFOFFSET);

    writel((readl(led_chrcdev->gpf3_base + CONFOFFSET) & ~(0xff<< 16)) | (0x11 << 16),led_chrcdev->gpf3_base + CONFOFFSET);

    return 0;
err5:
    iounmap(led_chrcdev->gpx2_base);
err4:
    iounmap(led_chrcdev->gpx1_base);
err3:
    cdev_del(&led_chrcdev->cdev);
err2:
    unregister_chrdev_region(devno,1);
err1:
    kfree(led_chrcdev);
    return ret;


}

static __exit void led_chardev_exit(void)
{
    iounmap(led_chrcdev->gpf3_base);
    iounmap(led_chrcdev->gpx2_base);
    iounmap(led_chrcdev->gpx1_base);
    cdev_del(&led_chrcdev->cdev);
    kfree(led_chrcdev);
    unregister_chrdev_region(MKDEV(led_chardev_major,0),DEVICE_NUMBER);
}


module_init(led_chardev_init);
module_exit(led_chardev_exit);
