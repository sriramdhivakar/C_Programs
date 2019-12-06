#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>

static int __init start(void)
{
	if(!gpio_is_valid(60))
	{
		printk(KERN_ERR"This gpiono is not valid\n");
		return -EINVAL;
	}
	gpio_request(60,"sysfs");
	gpio_export(60,false);
	gpio_direction_output(60,1);
	gpio_set_value(60,1);

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
