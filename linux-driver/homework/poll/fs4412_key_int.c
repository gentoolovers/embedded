#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/semaphore.h>
#include <linux/sched.h>
#include <linux/poll.h>


#define FS4412_KEY_CHRDEV_MAJOR 250
#define FS4412_KEY_CHRDEV_MINOR 0
#define FS4412_KEY_DEVICE_NUMBER 3
#define STRNCMPSIZE 12
#define BUFFERSIZE 128

MODULE_LICENSE("GPL");
static unsigned int  fs4412_key_chrdev_major =FS4412_KEY_CHRDEV_MAJOR;
static unsigned int fs4412_key_chrdev_minor=FS4412_KEY_CHRDEV_MINOR;
struct key_cdev
{
    char data[BUFFERSIZE];
    int len;
    wait_queue_head_t rq,wq;
    struct semaphore sem;
    struct cdev cdev;
};
static struct key_cdev *fs4412_chrdev;

char buffer[128] ;
struct key_platformdev
{

    struct resource *fs_key_res[8];
};

static struct key_platformdev *fs4412_key_cdev;



irqreturn_t fs4412_key_2_handler(int irqno, void *devid)
{
    printk("key2:happy hacking\n");
    memcpy(buffer,"key2:read",10);
    printk("irqno = %d\n", irqno);
    return IRQ_HANDLED;
}

irqreturn_t fs4412_key_3_handler(int irqno, void *devid)
{
    printk("key3:happy hacking\n");
    memcpy(buffer,"key3:read",10);
    printk("irqno = %d\n", irqno);
    return IRQ_HANDLED;
}
irqreturn_t fs4412_key_4_handler(int irqno, void *devid)
{
    printk("key4:happy hacking\n");
    memcpy(buffer,"key4:read",10);
    printk("irqno = %d\n", irqno);
    return IRQ_HANDLED;
}
irqreturn_t (*fs4412_key_2_and_3_handler[8])(int,void*)= {
    fs4412_key_2_handler,
    fs4412_key_3_handler,
};
static struct of_device_id key_table[] = {
    {.compatible = "fs4412,key_2_and_3",},
    {.compatible = "fs4412,key_4"},
    {},
};
static int fs4412_key_open(struct inode *inode,struct file *filp)
{


    //  filp->private_data =container_of(inode->i_cdev,struct key_cdev,cdev);
    printk("device opened\n");
    return 0;
}

static int  fs4412_key_release(struct inode *inode,struct file *filp)
{
    dev_t devno = MKDEV(fs4412_key_chrdev_major,fs4412_key_chrdev_minor);
    cdev_del(&(fs4412_chrdev->cdev));
    unregister_chrdev_region(devno,FS4412_KEY_DEVICE_NUMBER);
    kfree(fs4412_chrdev);
    return 0;
}

ssize_t fs4412_key_read(struct file *filp,char *buf,size_t count,loff_t *offp)
{

    ssize_t ret =0;
    struct key_cdev *dev = filp->private_data;
    down(&(dev->sem));
    while(dev->len == 0)
    {
        up(&dev->sem);
        if(filp->f_flags & O_NONBLOCK)
            return -EAGAIN;
        //当条件成立是返回0
        if(wait_event_interruptible(dev->rq,(dev->len != 0)))
            return -ERESTARTSYS;
        down(&(dev->sem));
    }
    if(count > dev->len)
        count = dev->len;
    if(copy_to_user(buf,buffer,count))
    {
        ret = -EFAULT;
    }else
    {
        printk("read %d bytes\n",count);
        dev->len -=count;
        ret =count;
        memcpy(dev->data,dev->data + count,dev->len);
    }
    up(&(dev->sem));
    wake_up(&(dev->wq));

    copy_to_user(buf,buffer,2);
    return ret;
}
static long fs4412_key_unlocked_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{

    return 0;
}
ssize_t fs4412_key_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp)
{
    ssize_t ret =0;
    struct key_cdev *dev= filp->private_data;
    if(count > BUFFERSIZE)
        return -ENOMEM;
    down(&dev->sem);
    while((BUFFERSIZE - dev->len) < count)
    {
        up(&dev->sem);
        if(filp->f_flags & O_NONBLOCK)
            return -EAGAIN;
        if(wait_event_interruptible(dev->wq,(BUFFERSIZE - dev->len) >=count))
            return -ERESTARTSYS;
        down(&dev->sem);
    }
    if(copy_from_user(dev->data + dev->len,buf,count))
    {
        return -EFAULT;
    }else
    {
        dev->len += count;
        ret = count;
    }
    up(&dev->sem);
    wake_up(&dev->rq);
    return 0;
}
unsigned int fs4412_key_poll(struct file *filp, struct poll_table_struct *poll_table)
{

    return 0;
}
static struct file_operations fs4412_key_chrdev_fops = {
    .owner = THIS_MODULE,
    .open  = fs4412_key_open,
    .release =fs4412_key_release,
    .unlocked_ioctl =fs4412_key_unlocked_ioctl,
    .read = fs4412_key_read,
    .write = fs4412_key_write,
    .poll  = fs4412_key_poll

};
static int fs4412_key_init(void)
{
    int ret=0;
    dev_t devno = MKDEV(fs4412_key_chrdev_major,fs4412_key_chrdev_minor);
    fs4412_chrdev=kzalloc(sizeof(struct key_cdev),GFP_KERNEL);
    if(!fs4412_chrdev)
    {
        ret= -ENOMEM;
        return ret;
    }

    if(fs4412_key_chrdev_major)
    {
        ret=register_chrdev_region(devno,FS4412_KEY_DEVICE_NUMBER,"fs4412_key");
        if(ret <0)
        {
            printk("fail:register_chrdev_region\n");
            goto err1;
        }
    }else
    {
        ret=alloc_chrdev_region(&devno,0,FS4412_KEY_DEVICE_NUMBER,"fs4412_key");
        if(ret <0 )
        {
            printk("fail:alloc_chrdev_region\n");
            goto err1;
        }
        fs4412_key_chrdev_major=MAJOR(devno);
        fs4412_key_chrdev_minor=MINOR(devno);
    }
    cdev_init(&(fs4412_chrdev->cdev),&fs4412_key_chrdev_fops);
    fs4412_chrdev->cdev.owner=THIS_MODULE;

    ret=cdev_add(&(fs4412_chrdev->cdev),devno,FS4412_KEY_DEVICE_NUMBER);
    if(ret < 0)
    {
        printk("failed:cdev_add\n");
        goto err2;
    }
    init_waitqueue_head(&fs4412_chrdev->rq);
    init_waitqueue_head(&fs4412_chrdev->wq);
    sema_init(&fs4412_chrdev->sem,1);
    memset(fs4412_chrdev->data,0,BUFFERSIZE);
    fs4412_chrdev->len=0;

    return 0;
err2:
    unregister_chrdev_region(devno,FS4412_KEY_DEVICE_NUMBER);
err1:
    kfree(fs4412_chrdev);
    return ret;
}
//MODULE_DEVICE_TABLE(of,platform_device_dt_table);
static int fs4412_key_probe(struct platform_device *pdev)
{
    int ret=0;
    int i=0;
    static int  flag=1;
    unsigned int irq_flags;

    printk("probe success\n");

    fs4412_key_cdev=kzalloc(sizeof(struct key_cdev),GFP_KERNEL);
    pdev->dev.platform_data = fs4412_key_cdev;
    for(i=0;i<pdev->num_resources;i++)
    {
        if((fs4412_key_cdev->fs_key_res[i] =platform_get_resource(pdev,IORESOURCE_IRQ,i)) == NULL)
        {
            continue;
        }else
        {

        }

    }
    for(i=0;i<pdev->num_resources;i++)
    {


        if(strncmp(fs4412_key_cdev->fs_key_res[i]->name+1,"fs4412_keysx",STRNCMPSIZE) == 0)
        {
            printk("!!!!!!!!!!!!!!!!!!!!!!!!\n");
            irq_flags = (fs4412_key_cdev->fs_key_res[i]->flags) & IRQF_TRIGGER_FALLING;
            if(( ret = request_irq(fs4412_key_cdev->fs_key_res[i]->start, fs4412_key_2_and_3_handler[i], IRQF_DISABLED|irq_flags, NULL,  pdev->dev.platform_data))< 0)
                return ret;
        }
        if(strncmp(fs4412_key_cdev->fs_key_res[i]->name+1,"fs4412_key_4",STRNCMPSIZE) == 0)
        {printk("####################\n");
            irq_flags = fs4412_key_cdev->fs_key_res[i]->flags & IRQF_TRIGGER_FALLING;
            if(( ret = request_irq(fs4412_key_cdev->fs_key_res[i]->start, fs4412_key_4_handler, IRQF_DISABLED|irq_flags, NULL,  pdev->dev.platform_data))< 0)
                return ret;
        }

    }

    if(flag)
    {
        flag--;
        return fs4412_key_init();
    }
    return 0;
}
static int fs4412_key_remove(struct platform_device* pdev)
{

    int i=0;
    struct resource *fs4412_key;
    printk("remove\n");
    for(i=0;i<pdev->num_resources;i++)
    {
        printk("remove %d\n",i);
        fs4412_key=platform_get_resource(pdev, IORESOURCE_IRQ, i);
        if(fs4412_key != NULL)
        {
            printk("%d\n",fs4412_key->start);
            free_irq(fs4412_key->start, pdev->dev.platform_data);
        }
    }


    kfree( pdev->dev.platform_data);
    return 0;
}
struct platform_driver fs4412_key_driver = {

    .driver = {
        .name = "fs4412-key",
        .of_match_table=of_match_ptr(key_table),
    },

    .probe= fs4412_key_probe,
    .remove=fs4412_key_remove,
};
static int __init fs4412_platform_init(void)
{
    platform_driver_register(&fs4412_key_driver);
    return 0;
}

static void __exit fs4412_platform_exit(void)
{
    platform_driver_unregister(&fs4412_key_driver);

}

module_init(fs4412_platform_init);
module_exit(fs4412_platform_exit);
