#include <linux/module.h>
#include <linux/kernel.h>

static int sno = 1;
module_param(sno,int,S_IRWXU);
MODULE_PARM_DESC(sno,"This is display in the /var/log");

static char *name = "sriram";
module_param(name,charp,S_IRWXU);
MODULE_PARM_DESC(name,"This is display inthe /var/log");

static long int rollno = 1000;
module_param(rollno,long,S_IRWXU);
MODULE_PARM_DESC(rollno,"This is display in the /var/log");

static int value[2] = {-1,1};
static int arg = 0;
module_param_array(value,int,&arg,S_IRWXU);
MODULE_PARM_DESC(value,"This is diplay in the /var/log");

static int __init start(void)
{
	int i;
	printk(KERN_INFO"The serial no is %d\n",sno);
	printk(KERN_INFO"The name of the candiate is %s\n",name);
	printk(KERN_INFO"The rollno is %ld\n",rollno);
	for(i=0;i<(sizeof(value)/sizeof(int));i++)
	{
		printk(KERN_INFO"The value[%d] is %d\n",i,value[i]);
	}
	printk(KERN_INFO"The argument passed is %d\n",arg);
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
MODULE_DESCRIPTION("This mdoule descripes the variable of the kernel");
