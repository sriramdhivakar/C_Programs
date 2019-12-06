#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>

static int __init start(void)
{
	int err;
	err = gpio_request_one(60,GPIOF_INIT_HIGH, "Reset Button");
	if(err)
	{
		printk(KERN_ERR"This Gpio is valid\n");
		return -EINVAL;
	}
	return 0;
}

static void __exit stop(void)
{
	gpio_set_value(60,0);
	gpio_unexport(60);
	gpio_free(60);
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
