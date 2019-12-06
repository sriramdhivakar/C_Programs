#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/init.h>

extern int value;
extern void func(void);

static int __init start(void)
{

	printk(KERN_INFO"File name is %s\n",__FILE__);
	printk(KERN_INFO"value is %d\n",value);
	func();
	return 0;
}

static void __exit stop(void)
{
	printk(KERN_INFO"Bye.....\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
