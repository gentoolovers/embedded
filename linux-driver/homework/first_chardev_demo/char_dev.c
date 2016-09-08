#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define DEVICE_NUMBER 1

MODULE_LICENSE("GPL");

static struct cdev cdev;

static int i =0;
module_param(i,int,0000);
//MODULE_PRAM_DESC(i,"this is a integer param");

static struct file_operations mychar_fops = {

    .owner=THIS_MODULE,
};
static int mychar_dev_init(void)
{
    int ret=-1;
    dev_t devno;
    ret=alloc_chrdev_region(&devno,0,1,"mychar_dev");
    if(ret <0)
    {
        printk("failed :register_chrdev_region\n");
        return ret;
    }
    cdev_init(&cdev,&mychar_fops);
    cdev.owner=THIS_MODULE;
    ret=cdev_add(&cdev,devno,DEVICE_NUMBER);
    if(ret <0)
    {
        printk("failed:cdev_add\n");
        return ret;
    }

    printk("mychar_dev_init\n");
    printk("%d\n",i);
    return 0;

}
static void mychar_dev_exit(void)
{
    cdev_del(&cdev);
    unregister_chrdev_region(cdev.dev,DEVICE_NUMBER);
    printk("mychar_dev_exit\n");
}

module_init(mychar_dev_init);
module_exit(mychar_dev_exit);
