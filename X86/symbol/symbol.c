#include <linux/module.h>
#include <linux/kernel.h>

static int value=500;
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
MODULE_AUTHOR("sriram");
MODULE_DESCRIPTION("This is symbol table then it can view in /proc");
