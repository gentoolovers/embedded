#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

MODULE_LICENSE("Dual BSD/GPL");

static void device_release(struct device *dev)
{
	printk("platform: device release\n");
}

struct platform_device test_device = {
	.id = -1,
	.name = "test_device",
	.dev.release = device_release, 
};

static int __init fs4412_platform_init(void)
{
	platform_device_register(&test_device);
	return 0;
}

static void __exit fs4412_platform_exit(void)
{
	platform_device_unregister(&test_device);
}

module_init(fs4412_platform_init);
module_exit(fs4412_platform_exit);
