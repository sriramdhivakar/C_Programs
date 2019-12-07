#include <linux/kernel.h>
#include <linux/module.h>

extern int value;
extern void func(void);

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
MODULE_AUTHOR("sriram");
MODULE_VERSION("0.1");
MODULE_DESCRIPTION("This module is used to get the value of the symbol table");
