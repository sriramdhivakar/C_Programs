#include <linux/kernel.h>
#include <linux/module.h>

static int value;
static void func(void);

EXPORT_SYMBOL_GPL(value);
EXPORT_SYMBOL_GPL(func);

static void func(void)
{
	printk(KERN_INFO"The value is %d\n",value);
}

static int __init start(void)
{
	func();
	return 0;
}

static void __exit stop(void)
{
	printk(KERN_INFO"Bye...\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("0.1");
MODULE_AUTHOR("sriramdhivakar.M(sriram.m@phytec.in)");
MODULE_DESCRIPTION("This module is used to explains the kernel symbol table");
