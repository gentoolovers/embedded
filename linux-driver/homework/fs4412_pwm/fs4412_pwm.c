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

struct fs4412_pwm* fs4412_pwm_timer;

static __init int fs4412_pwm_init(void)
{
    int ret=0;
    return ret;
}

static __exit void fs4412_pwm_exit(void)
{

}
module_init(fs4412_pwm_init);
module_exit(fs4412_pwm_exit);
