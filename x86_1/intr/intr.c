#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/interrupt.h>

#define SHARED_IRQ 28

static int irq = SHARED_IRQ,my_dev_id = 100,irq_counter = 0;
module_param(irq,int,S_IRUGO);

static irqreturn_t my_interrupt(int irq,void *dev_id)
{
	irq_counter++;
	printk(KERN_INFO"The No.of the count will take in the interrupt is %d\n",irq_counter);
	return IRQ_NONE;//interrupt was not from this device or was not handled(This is in the /usr/src/linux-4.7.6/include/linux/irqreturn.h )
}
static int __init start(void)
{
	/* request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,const char *name, void *dev)*/
	if(request_irq(irq,my_interrupt,IRQF_SHARED,"MY_Interrupt",&my_dev_id))
		return -1;
	printk(KERN_INFO"This interrupt id is %d\n",my_dev_id);
	return 0;
}

static void __exit stop(void)
{
	synchronize_irq(irq);
	free_irq(irq,&my_dev_id);
	printk(KERN_INFO"Interrupt is now unshared from the irq_table\n");
	printk(KERN_INFO"This device is successfully removed from the device\n");
}

module_init(start);
module_exit(stop);

MODULE_VERSION("I.0.1");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("sriram");
