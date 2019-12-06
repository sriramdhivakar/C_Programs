#nclude <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

static unsigned int gpioled = 49;
static unsigned int gpiosw = 115;
static unsigned int irqnumber;
static unsigned int numberpress = 0;
static bool ledon = false;

/*typedef irqreturn_t (*irq_handler_t)(int, void *)*/
static irq_handler_t gpio_irq_handler(unsigned int irq,void *dev_id,struct pt_regs *regs)
{
	ledon = !ledon;
	gpio_set_value(gpioled,ledon);
	printk(KERN_INFO"GPIO_TEST: Interrupt (button state is %d)\n",gpio_get_value(gpiosw));
	numberpress++;
	//return (irq_handler_t) IRQ_HANDLED;
	printk(KERN_INFO"GPIO_TEST\n");
	//return  IRQ_HANDLED;
	return  IRQ_NONE;
}

static int __init start(void)
{
	int result = 0;
	printk(KERN_INFO"GPIO_TEST: Initializing the GPIO_TEST LKM\n");

	if(!gpio_is_valid(gpioled))
	{
		printk(KERN_ERR"GPIO_TEST: invalid LED GPIO\n");
		return -ENODEV;
	}

	ledon = true;

	gpio_request(gpioled,"sysfs");
	gpio_direction_output(gpioled,ledon);
	gpio_export(gpioled,false);

	gpio_request(gpiosw,"sysfs");
	gpio_direction_input(gpiosw);
	gpio_set_debounce(gpiosw,200);
	gpio_export(gpiosw,false);

	printk(KERN_INFO"GPIO_TEST: The button state is currently: %d\n",gpio_get_value(gpiosw));

	irqnumber = gpio_to_irq(gpiosw);
	printk(KERN_INFO"GPIO_TEST:The button is mapped to IRQ: %d\n",irqnumber);

               /*request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,const char *name, void *dev)*/
	result = request_irq(irqnumber,(irq_handler_t)gpio_irq_handler,IRQF_TRIGGER_RISING,"gpio_handler",NULL);
	//result = request_irq(irqnumber,gpio_irq_handler,IRQF_TRIGGER_RISING,"gpio_handler",NULL);

	printk(KERN_INFO"GPIO_TEST: The interrupt requset result is %d\n",result);


	return result;
}

static void __exit stop(void)
{
	printk(KERN_INFO"GPIO_TEST: The button state is currently :%d\n",gpio_get_value(gpiosw));
	printk(KERN_INFO"GPIO_TEST: The button was pressed %d times\n",numberpress);
	gpio_set_value(gpioled,0);
	gpio_unexport(gpioled);
	free_irq(irqnumber,NULL);
	gpio_unexport(gpiosw);
	gpio_free(gpioled);
	gpio_free(gpiosw);
	printk(KERN_INFO"GPIO_TEST:Goodbye from the LKM\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("sriram");
MODULE_VERSION("0.1");
