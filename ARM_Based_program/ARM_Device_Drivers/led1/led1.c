#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>


static struct gpio leds_gpios[] = {
	{ 60, GPIOF_OUT_INIT_HIGH, "Power LED" }, 
};

static int __init start(void)
{

	int err;
	err = gpio_request_array(leds_gpios, ARRAY_SIZE(leds_gpios));
	if(err)
	{
		printk(KERN_ERR"This gpio is not valid\n");
		return -EINVAL;
	}
	return 0;
}

static void __exit stop(void)
{
	gpio_free_array(leds_gpios, ARRAY_SIZE(leds_gpios));
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
