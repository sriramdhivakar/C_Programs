#include <linux/kernel.h>
#include <linux/module.h>

static unsigned int led = 60;
static unsigned int sw  = 115;

static int __init start(void)
{
	return result;
}

static void __exit stop(void)
{
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
