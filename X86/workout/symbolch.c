#include <linux/module.h>
#include <linux/kernel.h>

static int *ptr;

static int __init start(void)
{
	ptr = (int*)__symbol_get("value");
	if(ptr)
	{
		*ptr = 700;
		__symbol_put("value");
	}
	else
	{
		printk(KERN_ERR"This value is not correct\n");
		return -EINVAL;
	}

	return 0;
}

static void __exit stop(void)
{
	printk(KERN_INFO"Bye...\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("sriramdhivakar.M(sriram.m@phytec.in)");
MODULE_VERSION("0.1");
MODULE_DESCRIPTION("This module is used to explain the symbol table value changing");
