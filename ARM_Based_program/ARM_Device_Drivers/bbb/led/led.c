#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h> 
static unsigned int gpioled = 9;

static int __init start(void)
{
	printk(KERN_INFO"GPIO_TEST: Initalization the GPIO_TEST LKM\n");
	
	if(!gpio_is_valid(gpioled))
	{
		printk(KERN_INFO"GPIO_TEST: invalid LED GPIO\n");
		return -ENODEV;
	}

	gpio_request(gpioled,"sysfs");
	gpio_export(gpioled,false);
	gpio_direction_output(gpioled,1);
	gpio_set_value(gpioled,0);

	return 0;
}

static void __exit stop(void)
{
	gpio_set_value(gpioled,0);
	gpio_unexport(gpioled);
	gpio_free(gpioled);
	printk(KERN_INFO"GPIO_TEST: Goodbye from the LKM\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("bv0.1");
MODULE_AUTHOR("sriram");
