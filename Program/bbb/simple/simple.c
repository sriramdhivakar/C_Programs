#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <asm/current.h>

static int sno = 1;
module_param(sno,int,S_IRWXU);
MODULE_PARM_DESC(sno,"This is display in /var/log\n");

static char *name = "sriram";
module_param(name,charp,S_IRWXU);
MODULE_PARM_DESC(name,"This is display in /var/log\n");

static int i[2] = {-1,1};
static int j = 0;
module_param_array(i,int,&j,S_IRWXU);
MODULE_PARM_DESC(i,"This is used to display the array in the /var/log\n");

static int __init start(void)
{
	printk(KERN_INFO"Hi this is %s for kernel\n",name);
	printk(KERN_INFO"My serial no %d\n",sno);
	printk(KERN_INFO"My kernel array %d and %d\n",i[0],i[1]);
	return 0;
}
static void __exit stop(void)
{
	printk(KERN_INFO"Bye......\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
