#include <linux/kernel.h>
#include <linux/module.h>

static int *ptr;

static int __init start(void)
{
	ptr = (int*)__symbol_get("value");
	if(ptr)
	{
		*ptr = 600;
		__symbol_put("value");
	}
	else
	{
		printk(KERN_ERR"This value is not equal\n");
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
MODULE_DESCRIPTION("This module is based on symbol value change");
MODULE_AUTHOR("sriram");
MODULE_VERSION("0.1");
