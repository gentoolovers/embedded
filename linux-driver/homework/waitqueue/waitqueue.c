#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/wait.h>
#define DATASIZE 128
MODULE_LICENSE ("GPL");

unsigned int hello_major = 250;
unsigned int hello_minor =0;
unsigned int number_of_devices=1;

struct hello_device
{
    char data[DATASIZE];
    int len;
    wait_queue_head_t rq,wq;
    struct semaphore sem;
    struct cdev cdev;
}hello_dev;

static int hello_open(struct inode *inode,struct file *filp)
{
    //指向hello_devices的首地址
    filp->private_data=container_of(inode->i_cdev,struct hello_device,cdev);
    printk("device opened\n");
    return 0;
}

static int hello_release(struct inode* inode,struct file *filp)
{
    printk("devices closed\n");
    return 0;
}

ssize_t hello_read(struct file *filp,char *buff,size_t count,loff_t *offp)
{
    ssize_t ret =0;
    struct hello_device *dev =filp->private_data;
    down(&dev->sem);
    while(hello_dev.len == 0)
    {
        up(&dev->sem);
        if(filp->f_flags & O_NONBLOCK)
            return -EAGAIN;
        if(wait_event_interruptible(dev->rq,(dev->len != 0)))
            return -ERESTARTSYS;
        down(&dev->sem);
    }
    if(count > dev->len)
        count = dev->len;
    if(copy_to_user (buff,dev->data,count))
    {
        ret = -EFAULT;
    }else
    {
        printk("read %d bytes\n",count);
        dev->len -= count;
        ret=count;
        memcpy(dev->data,dev->data+count,dev->len);
    }
    up(&dev->sem);
    wake_up(&dev->wq);
    return ret;
}

ssize_t hello_write(struct file *filp,const char *buf,size_t count,loff_t *offp)
{
    ssize_t ret =0;
    struct hello_device *dev =filp->private_data;
    printk("hechengyu\n");
    if(count > 128)
        return -ENOMEM;
    down(&dev->sem);
    while((DATASIZE - hello_dev.len) < count)
    {

        up(&dev->sem);
        if(filp->f_flags & O_NONBLOCK)
            return -EAGAIN;
        if(wait_event_interruptible(dev->wq,(dev->len != DATASIZE)));
        return -ERESTARTSYS;
        down(&dev->sem);
    }


    if(copy_from_user(dev->data+dev->len,buf,count))
    {
        ret = -EFAULT;
    }else
    {
        dev->len +=count;
        ret= count;
    }
    up(&dev->sem);
    wake_up(&dev->rq);
    return ret;
}
static struct file_operations hello_fops = {

    .owner = THIS_MODULE,
    .open  = hello_open,
    .release = hello_release,
    .read =hello_read,
    .write =hello_write,

};
static int __init hello_2_init(void)
{
    int ret;
    dev_t devno;
    devno = MKDEV(hello_major,hello_minor);
    ret=register_chrdev_region(devno,number_of_devices,"hello");
    if(ret < 0)
    {
        printk("waitqueue:register_chrdev_region fail\n");
        return ret;
    }

    cdev_init(&hello_dev.cdev,&hello_fops);
    hello_dev.cdev.owner = THIS_MODULE;
    if((ret = cdev_add(&hello_dev.cdev,devno,number_of_devices) ) < 0)
    {
        printk("waitqueue:cdev_add fail\n");
        goto error1;
    }

    init_waitqueue_head(&hello_dev.rq);
    init_waitqueue_head(&hello_dev.wq);
    sema_init(&hello_dev.sem,1);
    memset(hello_dev.data,0,DATASIZE);
    hello_dev.len = 0;
    printk("char devices registered\n");


    return 0;

error1:
    unregister_chrdev_region(devno,number_of_devices);
    return ret;
}

static void __exit hello_2_exit(void)
{
    dev_t devno = MKDEV(hello_major,hello_minor);
    unregister_chrdev_region(devno,number_of_devices);
    cdev_del(&hello_dev.cdev);

}

module_init(hello_2_init);
module_exit(hello_2_exit);
