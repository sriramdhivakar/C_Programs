#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>


static int __init start(void)
{
	struct task_struct *task;
	for_each_process(task){
		printk(KERN_INFO"process name %s,process id %d,process state %ld\n",task->comm,task->pid,task->state);
	};
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
MODULE_AUTHOR("sriramdhivakar.M(sriram.m@phytec.in)");
MODULE_DESCRIPTION("This module is used to explain the process working");
