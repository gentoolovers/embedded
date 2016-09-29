#ifndef __FS4412__KEY__H__
#define __FS4412__KEY__H__ 


#include <linux/cdev.h>
#include <linux/sched.h> 
#include <linux/wait.h>
enum state {
	FS4412_KEY_DOWN =1,FS4412_KEY_UP,
};

struct fs4412_key_dev {
	struct  cdev cdev;
	dev_t devno ; /*字符设备的设备号,动态申请*/
	struct class *cls; 
	int irq_nums; /*一个设备节点中中断的个数*/
	struct resource *fs4412_key_res[8];
	wait_queue_head_t rq;
	enum state state ; /*按鍵被按下的状态,当按键被按下时 可读*/
	unsigned int key_down_count; /*统计按键被按下的次数*/
	/*如何提高资源数量的灵活性，想起了共享内存中零长数组的使用*/
};



#endif 

