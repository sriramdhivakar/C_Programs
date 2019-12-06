#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#define irq_shared 28

static int irq = irq_shared,irq_counter = 0;
module_param(irq,int,S_IRUGO);


static irqreturn_t my_irq(int irq,void *dev_id)
{
	irq_counter++;
	printk(KERN_INFO"The Irq happened in the kernel are %d\n",irq_counter);
	return IRQ_NONE;
}

static int __init start(void)
{
	/*request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,const char *name, void *dev)*/
	if(request_irq(irq,my_irq,IRQF_SHARED,"MY_INTERRUPT",NULL))
		return -1;
	printk(KERN_INFO"Interrupt is now generated\n");
	return 0;
}

static void __exit stop(void)
{
	synchronize_irq(irq);
	free_irq(irq,NULL);
	printk(KERN_INFO"The interrupt is removed the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_VERSION("0.1");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("sriram");
