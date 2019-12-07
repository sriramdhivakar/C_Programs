#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include <asm/current.h>

static int __init start(void)
{
	struct task_struct* task;
	for_each_process(task) {
		printk(KERN_INFO"Process name =%s,Process id =%d,Process state=%ld\n",task->comm,task->pid,task->state);
	 };
	/*printk(KERN_INFO"Process name %s,process id %d,process state %ld\n",current->comm,current->pid,current->state);*/
	return 0;
}

static void __exit stop(void)
{
	printk(KERN_INFO"Bye...\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("0.1");
MODULE_AUTHOR("sriram");
MODULE_DESCRIPTION("This is used to display the all process held in the pc");
