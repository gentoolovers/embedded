#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <linux/of.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/fs.h>

#include "fs4412_key.h" 



static struct fs4412_key_dev *fs4412_key_dev;

irqreturn_t  fs4412_key_irq_handler(int irqno, void *dev_id)
{
	struct fs4412_key_dev *dev = dev_id; 
	
	dev->key_down_count++;

	printk("fs4412_key_irqno = %d\n",irqno);
	printk("Key:%s  down_times:%d",dev->fs4412_key_res[0]->name,dev->key_down_count);
	
	/*用于唤醒等待队列*/
	dev->state = FS4412_KEY_DOWN;
	wake_up_interruptible(&dev->rq);
	return IRQ_HANDLED;
}






static ssize_t key_read(struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
	struct fs4412_key_dev *dev = filp->private_data;

	if(filp->f_flags & O_NONBLOCK){
		return -EAGAIN;
	}
	/*按键被按下时，返回按键被按下的计数值， 在中断处理程序里唤醒*/
	if(wait_event_interruptible(dev->rq,(dev->state == FS4412_KEY_DOWN))){
		return -ERESTARTSYS;
	}
	/*检测到状态后将状态恢复*/
	dev->state = FS4412_KEY_UP ;
	if(copy_to_user(buf,&dev->key_down_count,sizeof(dev->key_down_count))){
		return -EFAULT;
	}

	printk("Success: %s\n",__func__);
	return 0;
}
static ssize_t key_write(struct file *filp, const char __user *buf, size_t size, loff_t *offset)
{
	printk("Success: %s\n",__func__);
	return 0;
}

static int key_open(struct inode *inode, struct file *filp)
{
	struct fs4412_key_dev *dev = container_of(inode->i_cdev,struct fs4412_key_dev,cdev);
	filp->private_data = dev; 
	printk("Success: %s\n",__func__);
	return 0;
}
static int key_release(struct inode *inode, struct file *filp)
{
	printk("Success: %s\n",__func__);
	return 0;
}

static struct file_operations key_fops ={
	.owner 		= THIS_MODULE,
	.read 		= key_read,
	.write  	= key_write,
	.open 		= key_open,
	.release 	= key_release,
};


static int fs4412_key_cdev_setup(void)
{
	int ret =0;
	struct fs4412_key_dev *dev = fs4412_key_dev;
	/*根据该设备中中断的数量,即按键的数量来申请设备号*/
	ret = alloc_chrdev_region(&dev->devno,0,dev->irq_nums,dev->fs4412_key_res[0]->name);
	if(ret){
		goto ERR1;
	}
	dev->cdev.owner = THIS_MODULE; 
	cdev_init(&dev->cdev,&key_fops);
	printk("key_cdev: Major:%d Minor:%d !\n",MAJOR(dev->devno),MINOR(dev->devno));
	ret = cdev_add(&dev->cdev,dev->devno,dev->irq_nums);
	if(ret){
		goto ERR2; 
	}
	
	return 0;

ERR2:
	unregister_chrdev_region(dev->devno,dev->irq_nums);
ERR1:
	return ret;
}




/*platform总线完成匹配工作后,会将platform_device传给probe() 函数*/
static int fs4412_probe(struct platform_device *dev)
{

	int ret = 0;
	int i=0,j=0;
	char cls_name[20];
	struct device *key_dev;
	unsigned long irq_flags; 
	/*kzalloc() 分配一片物理上连续的内存空间，GFP_KERNEL可能会引起睡眠*/
	fs4412_key_dev = kzalloc(sizeof(*fs4412_key_dev),GFP_KERNEL);
	if(!fs4412_key_dev){
		printk("Failed : kzalloc() in %d line!\n",__LINE__);
		ret = -ENOMEM;
		goto ERR1;
	}
	/*将分配的新结构体存在dev的私有成员platfrom_data中,因为可能有多次匹配*/
	dev->dev.platform_data = fs4412_key_dev ;
	
	/*初始化读等待队列*/
	init_waitqueue_head(&fs4412_key_dev->rq);

	printk("Success: %s:%d \n",__func__,__LINE__);
	for(i=0;i<dev->num_resources;i++){
		printk("dev->num_resources = %d\n",dev->num_resources);
		fs4412_key_dev->fs4412_key_res[fs4412_key_dev->irq_nums] = 
			platform_get_resource(dev,IORESOURCE_IRQ,i);
		if(!fs4412_key_dev->fs4412_key_res[fs4412_key_dev->irq_nums]){
			continue;
		}else{
			fs4412_key_dev->irq_nums++; /*存在問題：资源数量不能大于自定义数组的大小*/
		}
	}


	printk("name=%s irq_nums= %d\n",dev->resource->name,fs4412_key_dev->irq_nums);
	/***疑問：如何获得设备树中的中断触发方式资源***/
	for(i=0;i<fs4412_key_dev->irq_nums;i++){
	 	printk("Key_res: start=%d , end=%d\n",fs4412_key_dev->fs4412_key_res[i]->start,
				fs4412_key_dev->fs4412_key_res[i]->end);

		/* 此次中断触发方式应该从设备树里提取，而不应该是显示指定
		 * ret = request_irq(fs4412_key_dev->fs4412_key_res[i]->start,fs4412_key_irq_handler,
		 *		IRQF_DISABLED|IRQF_TRIGGER_FALLING,NULL,NULL);
		*/
			
		  /*从资源里提取中断为何种触发方式*/	
		  irq_flags = fs4412_key_dev->fs4412_key_res[i]->flags&IRQF_TRIGGER_MASK; 	
		  
		  /*注;最后一个元素可以为NULL,但此处了为在中断处理程序中判断是哪个按键按下故将传给中断处理函数*/
		  ret = request_irq(fs4412_key_dev->fs4412_key_res[i]->start,fs4412_key_irq_handler,
		 		IRQF_DISABLED|irq_flags,NULL,fs4412_key_dev);
		if(ret){
			printk("failed :request_irq() :%d\n",__LINE__);
			goto ERR2;
		}
	}
	
	ret = fs4412_key_cdev_setup();
	
	printk("In %d Lien ret = %d\n",__LINE__,ret);

	if(ret){
		goto ERR2;
	}
#if 1 	
	/*自动创建设备文件为应用程提供接口 需要在cdev_add() 前创建设备文件*/
	ret = snprintf(cls_name,sizeof(cls_name),"key%d",MAJOR(fs4412_key_dev->devno));
	//ret = snprintf(cls_name,sizeof(cls_name),"key%d%d",MAJOR(fs4412_key_dev->devno)
	//		,MINOR(fs4412_key_dev->devno));
	fs4412_key_dev->cls = class_create(THIS_MODULE,cls_name);
	if(IS_ERR(fs4412_key_dev->cls)){
		ret = PTR_ERR(fs4412_key_dev->cls);
		goto ERR3;
	}
	
	for(j=0;j<fs4412_key_dev->irq_nums;j++){
		key_dev = device_create(fs4412_key_dev->cls,NULL,fs4412_key_dev->devno,
				NULL,"key%d%d%d",MAJOR(fs4412_key_dev->devno),MINOR(fs4412_key_dev->devno),j);
		if(IS_ERR(key_dev)){
			ret = PTR_ERR(key_dev);
			goto ERR4;
		}
	}
#endif 
	return 0;
#if 1
ERR4:
	for(j--;j>=0;j++){
		device_destroy(fs4412_key_dev->cls,fs4412_key_dev->devno);
	}

	class_destroy(fs4412_key_dev->cls);
ERR3:
	cdev_del(&fs4412_key_dev->cdev);
#endif 
ERR2:
	for(i--;i>=0;i++){
		free_irq(fs4412_key_dev->fs4412_key_res[i]->start,NULL);
	}
	kfree(fs4412_key_dev);
ERR1:
	return ret;
	
}

static int fs4412_remove(struct platform_device *dev)
{
	int i=0;
	
	/*注意：删除时需要从platform_device 的私有成员platform_data 中提取出设备结构体*/
	struct fs4412_key_dev *key_dev = dev->dev.platform_data;
	
	cdev_del(&key_dev->cdev);

#if 1
	for(i=0;i<key_dev->irq_nums;i++){
		device_destroy(key_dev->cls,key_dev->devno);
	}
	class_destroy(key_dev->cls);
#endif 
	printk("fs4412_remove:irq_nums = %d\n",key_dev->irq_nums);
	for(i=0;i<key_dev->irq_nums;i++){
		//free_irq(key_dev->fs4412_key_res[i]->start,NULL);
		free_irq(key_dev->fs4412_key_res[i]->start,key_dev);
	}

	kfree(key_dev);
	printk("Success: %s:%d \n",__func__,__LINE__);
	return 0;
}

static struct of_device_id of_fs4412_ids[] ={
	{.compatible ="fs4412,key2"},
	{.compatible ="fs4412,key3"},
	{.compatible ="fs4412,key4"},
	{/*nothing to do*/},
};
MODULE_DEVICE_TABLE(of,of_fs4412_ids);

static struct platform_driver fs4412_platform_driver={
	.probe 	 	=fs4412_probe,
	.remove 	=fs4412_remove,
	.driver 	={
		.name  ="mach-fs4412", /*和platform_device匹配用*/
		.owner =THIS_MODULE,/*此处可以不写,因为注册宏有实现该初始化*/
		.of_match_table = of_match_ptr(of_fs4412_ids), /*设备树匹配用*/
	},

};




#if 1
module_platform_driver(fs4412_platform_driver);
#else /*{{{*/
static int __init fs4412_key_init(void)
{
	printk("Success: %s:%d \n",__func__,__LINE__);
	/*注册函数主要将平台设备或驱动添加到 platform总线上*/
	return platform_driver_register(&fs4412_platform_driver);
}

static void __exit fs4412_key_exit(void)
{
	printk("Success: %s:%d \n",__func__,__LINE__);
	platform_driver_unregister(&fs4412_platform_driver);
}
module_init(fs4412_key_init);
module_exit(fs4412_key_exit);
/*}}}*/
#endif 
MODULE_LICENSE("GPL");











