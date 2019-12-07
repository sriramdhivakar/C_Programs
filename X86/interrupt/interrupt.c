#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>

#define SHARED_IRQ 28

static int irq = SHARED_IRQ;
module_param(irq,int,S_IRWXU);
MODULE_PARM_DESC(irq,"This is displayed in the /var/log");

static int irqcount = 0;
static int my_irq = 500;

static irqreturn_t my_interrupt(int irq,void *dev_id)
{
	irqcount++;
	printk(KERN_INFO"This interrupt is held these many times %d\n",irqcount);
	return IRQ_NONE;
}

static int __init start(void)
{
	if(request_irq(irq,my_interrupt,IRQF_SHARED,"Sriram",&my_irq) < 0)
	{
		printk(KERN_ERR"This interrupt is not taken\n");
		return -EINVAL;
	}
	printk(KERN_INFO"The interrupt number is %d\n",irq);
	return 0;
}

static void __exit stop(void)
{
	free_irq(irq,&my_irq);
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("0.1");
MODULE_AUTHOR("sriram");

