#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/errno.h>

static int gpio = 1;

static int __init start(void)
{
	if(gpio_is_valid(gpio))
	{
		printk(KERN_INFO"The gpio N0 = %d is used for testing",gpio);
	}
	else
	{
		printk(KERN_INFO"This gpio is not valid\n");
		return -ENODEV;
	}

	gpio_request(gpio,"sysfs");
	gpio_export(gpio,0);
	gpio_direction_output(gpio,0);
	gpio_set_value(gpio,1);

	return 0;

}

static void __exit stop(void)
{
	gpio_set_value(gpio,0);
	gpio_unexport(gpio);
	gpio_free(gpio);
	printk(KERN_INFO"This device is successfully removed from the driver\n");
}

module_init(start);
module_exit(stop);


MODULE_LICENSE("Dual BSD/GPL");
