#include <linux/kernel.h>
#include <linux/module.h>
int init_module(void)
{
	printk(KERN_INFO"Hi\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO"Bye..\n");
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("sriramdhivakar.M(sriram.m@phytec.in)");
