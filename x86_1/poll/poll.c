#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/current.h>
#include <asm/uaccess.h>
/**********************************************************************************************************************************************/
#include <linux/wait.h>
#include <linux/poll.h>
/**********************************************************************************************************************************************/
#define CHAR_NAME "poll"
#define MAX_SIZE 2000

static dev_t first;
static int minor = 0;
static int count = 1;
static struct cdev *mydev;
static struct class *cls;
static char *kbuf;
static int balance;
static int inuse = 0;

/**********************************************************************************************************************************************/
static DECLARE_WAIT_QUEUE_HEAD(mywait);
static atomic_t data_ready;
/**********************************************************************************************************************************************/
static int open(struct inode *inode, struct file *filp)
{
	inuse++;
	printk(KERN_INFO"This device contains these many users %d\n",inuse);
	printk(KERN_INFO"The device process name %s and process id %d\n",current->comm,current->pid);
	printk(KERN_INFO"The device Majorno %d and Minorno %d\n",imajor(inode),iminor(inode));
	printk(KERN_INFO"The Refno = %d\n",module_refcount(THIS_MODULE));
	return 0;
}
static ssize_t write(struct file *filp, const char __user *buf, size_t ln, loff_t *pos)
{
	int nbytes,bytes_to_do;

	balance = MAX_SIZE - *pos;

	if(ln < balance)
	{
		bytes_to_do = ln;
	}
	else
	{
		bytes_to_do = balance;
	}

	if(bytes_to_do == 0)
	{
		printk(KERN_INFO"Reached the end of the device\n");
		return -ENOSPC;
	}

	nbytes = bytes_to_do - copy_from_user(kbuf+(*pos),buf,bytes_to_do);
	*pos += nbytes;
/**********************************************************************************************************************************************/
	atomic_set(&data_ready,1);
	wake_up_interruptible(&mywait);
/**********************************************************************************************************************************************/
	return nbytes;
}
static ssize_t read(struct file *filp, char __user *buf, size_t ln, loff_t *pos)
{
	int nbytes,bytes_to_do;

	balance = MAX_SIZE - *pos;

	if(ln < balance)
	{
		bytes_to_do = ln;
	}
	else
	{
		bytes_to_do = balance;
	}

	if(bytes_to_do == 0)
	{
		printk(KERN_INFO"Reached the end of the device \n");
		return -ENOSPC;
	}
/**********************************************************************************************************************************************/
	printk(KERN_INFO"Process name is %s and process id is %d\n",current->comm,current->pid);
	wait_event_interruptible(mywait,atomic_read(&data_ready));
	printk(KERN_INFO"Process name is %s and process id is %d\n",current->comm,current->pid);
	atomic_set(&data_ready,0);
/**********************************************************************************************************************************************/

	nbytes = bytes_to_do - copy_to_user(buf,kbuf+(*pos),bytes_to_do);
	*pos += nbytes;
	return nbytes;
}
static loff_t seek(struct file *filp, loff_t offset, int sk)
{
	loff_t testpos;

	switch(sk)
	{
		case 0://SEEK_SET
			testpos = offset;
			break;
		case 1://SEEK_CUR
			testpos = filp->f_pos + offset;
			break;
		case 2://SEEK_END
			testpos = MAX_SIZE + offset;
			break;
		default:
			return -EINVAL;
	}

	testpos = testpos < MAX_SIZE ? testpos : MAX_SIZE;
	testpos = testpos < 0 ? 0 : testpos;
	filp->f_pos = testpos;
	printk(KERN_INFO"SEEK value is %ld\n",(long)testpos);
	return testpos;
}

/**********************************************************************************************************************************************/
static unsigned int poll(struct file *filp, struct poll_table_struct *wait)
{
	poll_wait(filp,&mywait,wait);
	printk(KERN_INFO"Jiffies is %ld\n",jiffies);
	printk(KERN_INFO"data_ready is %d\n",data_ready);
	if(atomic_read(&data_ready))
	{
		return POLLIN|POLLRDNORM;
	}
	else
	{
		return 0;
	}
	return 0;
}
/**********************************************************************************************************************************************/
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
	.poll  = poll,
	.release = close,
};

static int __init start(void)
{
	if(alloc_chrdev_region(&first,minor,count,CHAR_NAME) < 0)
	{
		printk(KERN_ERR"This device is not register in the kernel\n");
		return -1;
	}
	if(!(mydev = cdev_alloc()))
	{
		unregister_chrdev_region(first,count);
		printk(KERN_ERR"This device is not allocated for cdev\n");
		return -1;
	}
	cdev_init(mydev, &f_ops);
	if(cdev_add(mydev,first,count) < 0)
	{
		cdev_del(mydev);
		unregister_chrdev_region(first,count);
		printk(KERN_ERR"This device is not added in the cdev\n");
		return -1;
	}
	cls = class_create(THIS_MODULE,"Myvirtual");
	device_create(cls,NULL,first,NULL,"%s",CHAR_NAME);
	kbuf = (char *)kzalloc(MAX_SIZE,GFP_KERNEL);
	printk(KERN_INFO"This device is created with the name of %s\n",CHAR_NAME);
	printk(KERN_INFO"The process name %s and process id %d\n",current->comm,current->pid);
	printk(KERN_INFO"The Majorno %d and Minorno %d\n",MAJOR(first),MINOR(first));
	return 0;
}

static void __exit stop(void)
{
	kfree(kbuf);
	device_destroy(cls,first);
	class_destroy(cls);
	cdev_del(mydev);
	unregister_chrdev_region(first,count);
	printk(KERN_INFO"This device is successfully removed from the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("0.1");
MODULE_AUTHOR("sriram");
