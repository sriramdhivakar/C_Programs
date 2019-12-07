#include <linux/kernel.h>
#include <linux/module.h>

int init_module(void)
{
	printk(KERN_INFO"Hi this is sriramdhivakar.M\n");
	return 0;
}

void clean_module(void)
{
	printk(KERN_INFO"Bye...\n");
}

MODULE_AUTHOR("SRIRAM");
MODULE_VERSION("0.1");
MODULE_DESCRIPTION("This is sriram from Linux");

