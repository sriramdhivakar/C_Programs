#include <linux/module.h>
#include <linux/kernel.h>

static int __init start(void)
{
	printk(KERN_INFO"This is sriram from user space\n");
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
MODULE_DESCRIPTION("This is new method of kernel");
