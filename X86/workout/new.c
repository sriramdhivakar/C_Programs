#include <linux/kernel.h>
#include <linux/module.h>

static int __init start(void)
{
	printk(KERN_INFO"Hi this is sriram from the kernel\n");
	return 0;
}

static void __exit stop(void)
{
	printk(KERN_INFO"Bye....\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("0.1");
MODULE_AUTHOR("sriramdhivakar.M(sriram.m@phytec.in)");
MODULE_DESCRIPTION("This module is new method of writing the kernel module");
