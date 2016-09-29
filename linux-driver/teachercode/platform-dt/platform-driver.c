#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/platform_device.h>

MODULE_LICENSE("Dual BSD/GPL");

static int driver_probe(struct platform_device *dev)
{
	printk("platform: match ok!\n");
	printk("resource start = 0x%x\n", dev->resource[0].start);
	return 0;
}

static int driver_remove(struct platform_device *dev)
{
	printk("platform: driver remove\n");
	return 0;
}

static struct of_device_id platform_device_dt_table[] = {
	{.compatible = "fs4412,dttest",},
	{/*noting to be done*/},
}; 
MODULE_DEVICE_TABLE(of, platform_device_dt_table);
struct platform_driver test_driver = {
	.probe = driver_probe,
	.remove = driver_remove,
	.driver = {
		.name = "test_device",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(platform_device_dt_table),
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
