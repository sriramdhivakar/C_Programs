#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>

static int value = 555;
static void func(void);

EXPORT_SYMBOL_GPL(value);
EXPORT_SYMBOL_GPL(func);

static void func(void)
{

	printk(KERN_INFO"Hi iam in Kernel\n");
	printk(KERN_INFO"value is %d\n",value);
}
static int __init start(void)
{
	printk(KERN_INFO"This is kernel symbol table\n");
	printk(KERN_INFO"value is %d\n",value);
	func();
	return 0;
}

static void __exit stop(void)
{

	printk(KERN_INFO"Bye..\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
