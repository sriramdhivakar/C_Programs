#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

static unsigned int led = 60;
static unsigned int sw = 115;
static unsigned int irq_number;
static int my_id = 100;
static bool ledon = false;
static int irqnum;

static irqreturn_t gpio_irq_handler(int irq,void *dev_id)
{
	ledon = !ledon;
	gpio_set_value(led,ledon);
	irqnum++;
	printk(KERN_INFO"The Button state is %d\n",gpio_get_value(sw));
	printk(KERN_INFO"The Button is pressed for these many times %d\n",irqnum);
	return IRQ_HANDLED;
}


static int __init start(void)
{
	int result = 0;
	if(!gpio_is_valid(led))
	{
		printk(KERN_ERR"This gpio%d is not valid\n",led);
		return -EINVAL;
	}

	gpio_request(led,"sysfs");
	gpio_export(led,false);
	gpio_direction_output(led,0);
	//gpio_set_value(led,1);

	gpio_request(sw,false);
	gpio_direction_input(sw);
	gpio_set_debounce(sw,200);

	printk(KERN_INFO"The Button state currently is %d\n",gpio_get_value(sw));
	irq_number = gpio_to_irq(sw);
	printk(KERN_INFO"The button generated irq number is %d\n",irq_number);

	result = request_irq(irq_number,(irq_handler_t)gpio_irq_handler,IRQF_TRIGGER_RISING,"My_gpio_interrupt",&my_id);
	printk(KERN_INFO"The Gpio based interrupt request is %d\n",result);
	return result;
}

static void __exit stop(void)
{
	gpio_set_value(led,0);
	gpio_unexport(sw);
	gpio_unexport(led);
	free_irq(irq_number,&my_id);
	gpio_free(sw);
	gpio_free(led);
	printk(KERN_INFO"This gpio is released from the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
