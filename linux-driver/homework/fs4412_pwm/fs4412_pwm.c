#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include "fs4412_pwm.h"

MODULE_LICENSE("GPL");

static int fs4412_pwm_major=FS4412_PWM_MAJOR;

struct fs4412_pwm
{
    struct cdev cdev;
    void __iomem* gpd0_base;
    void __iomem* tcfg0_base;
};

struct fs4412_pwm* fs4412_pwm_dev;


static int fs4412_pwm_open(struct inode* inode,struct file *file)
{

    writel((readl(fs4412_pwm_dev->gpd0_base) & ~(0xf << 0)) | (0x2 << 0), fs4412_pwm_dev->gpd0_base);

    //一级预分频 TCFG0
    writel((readl(fs4412_pwm_dev->tcfg0_base ) & ~(0xff << 0)) | 99, fs4412_pwm_dev->tcfg0_base);
    //二级 TCFG1
    writel((readl(fs4412_pwm_dev->tcfg0_base +4 ) & ~(0xf << 0)), fs4412_pwm_dev->tcfg0_base + 4);

    //TCON
    writel((readl(fs4412_pwm_dev->tcfg0_base +8 ) & ~(0x1 << 3)), fs4412_pwm_dev->tcfg0_base + 8);
    //设置总宽度 TCNTB0
    writel(1950, fs4412_pwm_dev->tcfg0_base + 12);
    //设置脉冲宽度值TCMPB0
    writel(500, fs4412_pwm_dev->tcfg0_base + 16);
    //手动装载脉冲总宽度值到计数寄存器 TCON
    writel((readl(fs4412_pwm_dev->tcfg0_base +8 ) |(0x1 << 1)), fs4412_pwm_dev->tcfg0_base + 8);
    //使能PWM计数器 TCON
    writel((readl(fs4412_pwm_dev->tcfg0_base +8 ) |(0x1)), fs4412_pwm_dev->tcfg0_base + 8);
    //关闭手动使能 TCON
    writel((readl(fs4412_pwm_dev->tcfg0_base +8 ) & ~(0x1 <<1)), fs4412_pwm_dev->tcfg0_base + 8);
    return 0;
}

static struct fs4412_pwm_on()
{
    writel((readl(fs4412_pwm_dev->tcfg0_base +8 ) |(0x1)), fs4412_pwm_dev->tcfg0_base + 8);
    //关闭手动使能 TCON
    writel((readl(fs4412_pwm_dev->tcfg0_base +8 ) & ~(0x1 <<1)), fs4412_pwm_dev->tcfg0_base + 8);

}
static struct fs4412_pwm_off()
{
//使能PWM计数器 TCON
    writel((readl(fs4412_pwm_dev->tcfg0_base +8 ) |(0x0)), fs4412_pwm_dev->tcfg0_base + 8);
}
static struct file_operations fs4412_pwm_fops = {
    .owner  =  THIS_MODULE,
    .open   = fs4412_pwm_open,
};
static long fs4412_pwm_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

    int nr;

    if (_IOC_TYPE(cmd) !='P')
        return -EINVAL;
    if (_IOC_NR(cmd) > 3)
        return -EINVAL;
    if(copy_from_user(&nr,(void*)arg,_IOC_SIZE(cmd)))
        return -EINVAL;

    switch(cmd)
    {

    case PWM_ON:
        fs4412_pwm_on();
        break;
    case SET_PRE:
        //一级预分频 TCFG0
        writel((readl(fs4412_pwm_dev->tcfg0_base ) & ~(0xff << 0)) | 99, fs4412_pwm_dev->tcfg0_base);
        //二级 TCFG1
        writel((readl(fs4412_pwm_dev->tcfg0_base +4 ) & ~(0xf << 0)), fs4412_pwm_dev->tcfg0_base + 4);
        break;
    case SET_CNT:
        //设置脉冲宽度值TCMPB0
        writel(500, fs4412_pwm_dev->tcfg0_base + 16);
        break;
    case PWM_OFF:
        fs4412_pwm_off();
        break;
    default:
        return -EINVAL;
    }
    return 0;
}


static __init int fs4412_pwm_init(void)
{
    int ret=0;
    dev_t devno=MKDEV(fs4412_pwm_major,FS4412_PWM_MINOR);
    fs4412_pwm_dev = kmalloc(sizeof(struct fs4412_pwm),GFP_KERNEL);
    if(!fs4412_pwm_dev)
    {
        ret=-ENOMEM;
        return ret;
    }
    if(fs4412_pwm_dev)
    {
        ret = register_chrdev_region(devno,FS4412_PWM_NUMBER,"fs4412_led");
        if(ret <0)
        {
            printk("failed:register_chrdev_region\n");
            goto err1;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&devno,0,FS4412_PWM_NUMBER,"fs4412_led");
        if(ret <0)
        {
            printk("failed:alloc_chrdev_region\n");
            goto err1;
        }
    }
    cdev_init(&fs4412_pwm_dev->cdev, &fs4412_pwm_fops);
    fs4412_pwm_dev->cdev.owner = THIS_MODULE;
    ret = cdev_add(&fs4412_pwm_dev->cdev, devno,FS4412_PWM_NUMBER);
    if (ret < 0)
    {
        printk("failed: cdev_add\n");
        goto err2;
    }

    fs4412_pwm_dev->gpd0_base = ioremap(GPD0_BASE,0x4);
    if (fs4412_pwm_dev->gpd0_base == NULL)
    {
        ret = -ENOMEM;
        printk("failed: ioremap gpd0_base\n");
        goto err3;
    }
    fs4412_pwm_dev->tcfg0_base=ioremap(TCFG0_BASE,0x14);
    if (fs4412_pwm_dev->gpd0_base == NULL)
    {
        ret = -ENOMEM;
        printk("failed: ioremap gpd0_base\n");
        goto err4;
    }
    return ret;
err4:
    iounmap(fs4412_pwm_dev->gpd0_base);
err3:
    cdev_del(&fs4412_pwm_dev->cdev);

err2:
    unregister_chrdev_region(devno, 1);

err1:
    kfree(fs4412_pwm_dev);
    return ret;


}

static __exit void fs4412_pwm_exit(void)
{
    iounmap(fs4412_pwm_dev->tcfg0_base);
    iounmap(fs4412_pwm_dev->gpd0_base);
    cdev_del(&fs4412_pwm_dev->cdev);
    kfree(fs4412_pwm_dev);
    unregister_chrdev_region(MKDEV(fs4412_pwm_major,0),FS4412_PWM_NUMBER);

}
module_init(fs4412_pwm_init);
module_exit(fs4412_pwm_exit);
