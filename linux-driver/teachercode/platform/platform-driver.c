#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

MODULE_LICENSE("Dual BSD/GPL");

static int driver_probe(struct platform_device *dev)
{
	printk("platform: match ok!\n");
	return 0;
}

static int driver_remove(struct platform_device *dev)
{
	printk("platform: driver remove\n");
	return 0;
}

struct platform_driver test_driver = {
	.probe = driver_probe,
	.remove = driver_remove,
	.driver = {
		.name = "test_device",
	},
};

static int __init fs4412_platform_init(void)
{
	platform_driver_register(&test_driver);
	return 0;
}

static void __exit fs4412_platform_exit(void)
{
	platform_driver_unregister(&test_driver);
}

module_init(fs4412_platform_init);
module_exit(fs4412_platform_exit);
