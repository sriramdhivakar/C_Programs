#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>

static int *ptr;


static int __init start(void)
{
	ptr = (int *)__symbol_get("value");
	if(ptr)
	{
		*ptr = 666;
		__symbol_put("value");
		return 0;
	}
	else
	{
		printk(KERN_INFO"This is not in the symbol table\n");
		return -1;
	}
}

static void __exit stop(void)
{
	printk(KERN_INFO"Bye\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
