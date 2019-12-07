#include <linux/kernel.h>
#include <linux/module.h>

int init_module(void)
{
	printk(KERN_INFO"Hai this is sriam from user space\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO"Bye..\n");
}

MODULE_VERSION("0.1");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("sriamdhivakar.M(sriram.m@phytec.in)");
MODULE_DESCRIPTION("This module explains the old method of writing the module");
