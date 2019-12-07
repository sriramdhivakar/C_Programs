#include <linux/kernel.h>
#include <linux/module.h>

static int sno = 1;
module_param(sno,int,S_IRWXU);
MODULE_PARM_DESC(sno,"This module is display in the /var/log");

static char *name = "sriram";
module_param(name,charp,S_IRWXU);
MODULE_PARM_DESC(name,"This module is display in the /var/log");

static long int rollno = 1000;
module_param(rollno,long,S_IRWXU);
MODULE_PARM_DESC(rollno,"This module is display in the /var/log");

static int value[2] = {-1,1};
static int arg = 0;
module_param_array(value,int,&arg,S_IRWXU);
MODULE_PARM_DESC(value,"This module is display in the /var/log");

static int __init start(void)
{
	int i;
	printk(KERN_INFO"The serial no: %d\n",sno);
	printk(KERN_INFO"The Name of the candidate: %s\n",name);
	printk(KERN_INFO"Rollno of the candidate: %ld\n",rollno);
	for(i=0;i<(sizeof(value)/sizeof(int));i++)
	{
		printk(KERN_INFO"The value[%d] is %d\n",i,value[i]);
	}
	printk(KERN_INFO"The no of argument passed is %d\n",arg);
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
MODULE_DESCRIPTION("This module is used for variable allocation in the kernel");
