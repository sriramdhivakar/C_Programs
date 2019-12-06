#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/sched.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <asm/current.h>


static int gpioled = 49;
static int gpiosw = 115;
static int led = 0 ;

static int __init start(void)
{
	/*printk(KERN_INFO"GPIO_TEST:Begin with the gpio value\n");

	if(!(gpio_is_valid(gpioled)))
	{
		printk(KERN_INFO"GPIO_TEST:This gpio is not valid\n");
		return -ENODEV;
	}

	gpio_request(gpioled,"sysfs");
	gpio_direction_output(gpioled,0);
	gpio_export(gpioled,true);*/

	int result;

	if(!(gpio_is_valid(gpiosw)))
	{
		printk(KERN_INFO"GPIO_SW:This gpio is not valid\n");
		return -ENODEV;
	}
	gpio_request(gpiosw,"sysfs");
	gpio_direction_input(gpiosw);
	gpio_set_debounce(gpiosw,200);//200ms
	gpio_export(gpiosw,true);
	result = gpio_get_value(gpiosw);

	printk(KERN_INFO"GPIO_TEST:GPIOvalue is %d\n",result);

	return 0;
}

static void __exit stop(void)
{
	/*gpio_set_value(gpioled,0);
	gpio_unexport(gpioled);
	gpio_free(gpioled);*/
	gpio_unexport(gpiosw);
	gpio_free(gpiosw);
	printk(KERN_INFO"GPIO_TEST:gpioled is removed the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_VERSION("b0.1");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("sriram");

