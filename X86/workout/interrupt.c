#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>

#define SHARED_IRQ 28

static int irq = SHARED_IRQ;
module_param(irq,int,S_IRWXU);
MODULE_PARM_DESC(irq,"This is used to display in the /var/log");

static int irqcount;
static int my_id = 100;

static irqreturn_t my_interrupt(int irq, void *dev_id)
{
	irqcount++;
	printk(KERN_INFO"This interrupt happened these many times %d\n",irqcount);
	return IRQ_NONE;
}

static int __init start(void)
{
	if(request_irq(irq,my_interrupt,IRQF_SHARED,"My_INTERRUPT",&my_id) < 0)
	{
			printk(KERN_ERR"This interrupt is not allowed in the kernel\n");
			return -EINVAL;
	}
	printk(KERN_INFO"This interrupt no is %d\n",irq);
	return 0;
}

static void __exit stop(void)
{
	free_irq(irq,&my_id);
	printk(KERN_INFO"This interrupt is removed from the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_VERSION("0.1");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("sriramdhivakar.M(sriram.m@phytec.in)");
MODULE_DESCRIPTION("This is describes the interrupt working");
