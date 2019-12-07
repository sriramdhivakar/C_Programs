#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
	printk(KERN_INFO"Hai this is sriram from user space\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO"Bye...\n");
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("0.1");
MODULE_AUTHOR("sriram");
MODULE_DESCRIPTION("This module is for beginners");
