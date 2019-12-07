#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/uaccess.h>

#define MAX_SIZE 2048
#define CHAR_NAME "auto"

static dev_t first;
static int minor = 0;
static int count = 1;
static struct cdev *mydev;
static struct class *cl;
static char *kbuf;
static int inuse=0;
static int balance;

static int open(struct inode *inode, struct file *filp)
{
	inuse++;
	printk(KERN_INFO"This driver is open for these many times %d\n",inuse);
	printk(KERN_INFO"Majorno %d,Minorno %d\n",imajor(inode),iminor(inode));
	printk(KERN_INFO"process name %s,process id %d,process state %ld\n",current->comm,current->pid,current->state);
	printk(KERN_INFO"The Refno=%d\n",module_refcount(THIS_MODULE));
	return 0;
}

static ssize_t write(struct file *filp, const char __user *buf, size_t ln, loff_t *pos)
{
	int nbytes,byte_do_to;
	balance = MAX_SIZE - *pos;
	if(ln < balance)
	{
		byte_do_to = ln;
	}
	else
	{
		byte_do_to = balance;
	}

	if(byte_do_to == 0)
	{
		printk(KERN_ERR"This driver reached the end\n");
		return -ENOSPC;
	}
	nbytes = byte_do_to - copy_from_user(kbuf+(*pos),buf,byte_do_to);
	*pos	+=	nbytes;
	return nbytes;
}

static ssize_t read(struct file *filp, char __user *buf, size_t ln, loff_t *pos)
{
	int nbytes,byte_do_to;
	balance = MAX_SIZE - *pos;
	if(ln < balance)
	{
		byte_do_to = ln;
	}
	else
	{
		byte_do_to = balance;
	}
	if(byte_do_to == 0)
	{
		printk(KERN_ERR"This driver reached the end\n");
		return -ENOSPC;
	}
	nbytes = byte_do_to - copy_to_user(buf,kbuf+(*pos),byte_do_to);
	*pos	+=	nbytes;
	return nbytes;
}

static int close(struct inode *inode, struct file *filp)
{
	inuse = 0;
	printk(KERN_INFO"This driver is ejected from the kernel\n");
	return 0;
}

static struct file_operations f_ops = {
	.owner	=	THIS_MODULE,
	.open	=	open,
	.write	=	write,
	.read	=	read,
	.release=	close,
};

static int __init start(void)
{
	if(alloc_chrdev_region(&first,minor,count,CHAR_NAME) < 0)
	{
		unregister_chrdev_region(first,count);
		printk(KERN_ERR"This driver is not register in the kernel\n");
		return -EINVAL;
	}
	mydev = cdev_alloc();
	cdev_init(mydev,&f_ops);
	if(cdev_add(mydev,first,count) < 0)
	{
		cdev_del(mydev);
		unregister_chrdev_region(first,count);
		printk(KERN_ERR"This driver is not register in the kernel\n");
		return -EINVAL;
	}
	cl = class_create(THIS_MODULE,"MY_DRIVER");
	device_create(cl,NULL,first,NULL,"%s",CHAR_NAME);
	kbuf = (char*)kzalloc(MAX_SIZE,GFP_KERNEL);
	printk(KERN_INFO"This driver is register with the name %s\n",CHAR_NAME);
	printk(KERN_INFO"Majorno %d,Minorno %d\n",MAJOR(first),MINOR(first));
	printk(KERN_INFO"process name %s,process id %d,process state %ld\n",current->comm,current->pid,current->state);
	return 0;
}

static void __exit stop(void)
{
	kfree(kbuf);
	device_destroy(cl,first);
	class_destroy(cl);
	cdev_del(mydev);
	unregister_chrdev_region(first,count);
	printk(KERN_INFO"This driver is removed from the kernel\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("sriramdhiavkar.M(sriram.m@phytec.in)");
MODULE_VERSION("0.1");
MODULE_DESCRIPTION("This driver is used to insert tne module in automatic method");
