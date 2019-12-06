#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/gpio.h>


static unsigned int gpio = 1;

static int __init start(void)
{
	printk(KERN_INFO"Led gpio test for ARIETTA G25 Board\n");

	if(!(gpio_is_valid(gpio)))
	{
		printk(KERN_INFO"Gpio led is not valid\n");
		return -ENODEV;
	}

	gpio_request(gpio,"sysfs");
	gpio_export(gpio,1);
	gpio_direction_output(gpio,1);
	gpio_set_value(gpio,1);

	return 0;
}

static void __exit stop(void)
{
	gpio_set_value(gpio,0);
	gpio_unexport(gpio);
	gpio_free(gpio);
	printk(KERN_INFO"Gpio led is now released from the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
