#include <linux/module.h>
#include <linux/kernel.h>

static int __init sri(void)
{
	printk(KERN_INFO"This is sriram\n");
	return 0;
}

static void __exit ram(void)
{
	printk(KERN_INFO"Byeee..\n");
}

module_init(sri);
module_exit(ram);

MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
MODULE_DESCRIPTION("This is my second module");
