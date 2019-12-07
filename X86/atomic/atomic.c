#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/atomic.h>


static int __init start(void)
{
	atomic_t av = ATOMIC_INIT(10);
	printk(KERN_INFO"The Atomic read %d\n",atomic_read(&av));
	atomic_add(10,&av);
	printk(KERN_INFO"The Atomic read %d\n",atomic_read(&av));
	atomic_sub(10,&av);
	printk(KERN_INFO"The Atomic read %d\n",atomic_read(&av));
	return 0;
}

static void __exit stop(void)
{
	printk(KERN_INFO"Bye..\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("0.1");
MODULE_AUTHOR("sriram");
MODULE_DESCRIPTION("This is driver is used to show the atomic working");
