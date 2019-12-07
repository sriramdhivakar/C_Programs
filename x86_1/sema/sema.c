#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <asm/current.h>
#include <asm/uaccess.h>
/**************************************SEMAPHORE Include and structure*************************************/
#include <linux/semaphore.h>
static struct semaphore mysem;
/**********************************************************************************************************/
#define CHAR_NAME "sriram"
#define MAX_SIZE 2000

static dev_t first;
static int count = 1;
static struct cdev *mydev;
static char *kbuf;
static int minor = 0;
static int inuse = 0;
static int balance;
static struct class *cl;

static int open(struct inode *inode, struct file *filp)
{
	inuse++;
	printk(KERN_INFO"This contain these many users %d\n",inuse);
	printk(KERN_INFO"MAJORNO %d and MINORNO %d\n",imajor(inode),iminor(inode));
	printk(KERN_INFO"This device process name %s and process id %d\n",current->comm,current->pid);
	printk(KERN_INFO"The refno is %d\n",module_refcount(THIS_MODULE));
	balance = sizeof(kbuf);
	printk(KERN_INFO"balance in the open is %d\n",balance);
	return 0;
}
static ssize_t write(struct file *filp, const char __user *buf, size_t ln, loff_t *pos)
{
	int nbyte = 0;
	printk(KERN_INFO"write contains %d amount of size with the data of %s\n",(int)ln,buf);
	if(ln < MAX_SIZE)
	{
		nbyte = copy_from_user(kbuf+(*pos),buf,ln);
		balance = ln;
		*pos += nbyte;
/**************************************SEMAPHORE***********************************************************/
		up(&mysem);
/**********************************************************************************************************/
		return ln;
	}
	else
	{
		printk(KERN_INFO"User asked too large amount of size\n");
		return 0;
	}
}
static ssize_t read(struct file *filp, char __user *buf, size_t ln, loff_t *pos)
{
	int nbyte,tmp;
	if(balance == 0)
	{
		return 0;
	}
/**************************************SEMAPHORE***********************************************************/
	printk (KERN_INFO "process %i, %s going to sleep\n", current->pid,current->comm);
	//if(down_interruptible(&mysem))
	//down(&mysem);
	//if(down_killable(&mysem))
	if(down_trylock(&mysem))
	{
		printk(KERN_INFO"currnet process id is %d\n",current->pid);
		return -ERESTARTSYS;
	}
/**********************************************************************************************************/
	if(ln < balance)
	{
		nbyte = copy_to_user(buf,kbuf+(*pos),ln);
		balance -= ln;
		*pos += ln;
		return ln;
	}
	else
	{
		nbyte = copy_to_user(buf,kbuf+(*pos),balance);
		tmp = balance;
		balance = 0;
		*pos = 0;
		return tmp;
	}
}
static loff_t seek(struct file *filp, loff_t pos, int sk)
{
	loff_t testpos;

	switch(sk)
	{
		case 0://SEEK_SET
			testpos = pos;
			break;
		case 1://SEEK_CUR
			testpos = filp->f_pos + pos;
			break;
		case 2://SEEK_END
			testpos = MAX_SIZE + pos;
			break;
		default:
			return -EINVAL;
	}
	testpos = testpos < MAX_SIZE ? testpos : MAX_SIZE;
	testpos = testpos < 0 ? 0 : testpos;
	filp->f_pos = testpos;
	printk(KERN_INFO"SEEk is %ld\n",(long)testpos);
	return testpos;
}
static int close(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO"This device is done the job\n");
	return 0;
}

static struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open  = open,
	.write = write,
	.read  = read,
	.llseek= seek,
	.release = close,
};

static int __init start(void)
{
	if(alloc_chrdev_region(&first,minor,count,CHAR_NAME) < 0)
	{
		printk(KERN_INFO"This device is not register properly\n");
		return -1;
	}		
	mydev = cdev_alloc();
	if(!(mydev = cdev_alloc()))
	{
		printk(KERN_INFO"This device is not alloated in the cdev\n");
		unregister_chrdev_region(first,count);
		return -1;
	}
	cdev_init(mydev,&f_ops);
	if(cdev_add(mydev,first,count) < 0)
	{
		printk(KERN_INFO"This driver is not allocated in the char_driver\n");
		cdev_del(mydev);
		unregister_chrdev_region(first,count);
		return -1;
	}
	cl = class_create(THIS_MODULE,"Myvirtual");
	device_create(cl,NULL,first,NULL,"%s",CHAR_NAME);
	printk(KERN_INFO"This device is register in the kernel directory with the  name %s\n",CHAR_NAME);
	printk(KERN_INFO"The process name %s and the process id %d of the device\n",current->comm,current->pid);
	printk(KERN_INFO"Majorno %d and the minorno %d\n",MAJOR(first),MINOR(first));
	kbuf = (char *)kzalloc(MAX_SIZE,GFP_KERNEL);
/**********************************SEMAPHORE initilization*************************************/
	sema_init(&mysem,0);
/**********************************************************************************************/
	return 0;
}

static void __exit stop(void)
{
	device_destroy(cl,first);
	class_destroy(cl);
	cdev_del(mydev);
	unregister_chrdev_region(first,count);
	printk(KERN_INFO"This device is removed from the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
