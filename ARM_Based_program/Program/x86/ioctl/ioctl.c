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
#include "ioctl_commands.h"

#define CHAR_NAME "sriram"
#define MAX_SIZE cln

static dev_t first;
static int count = 1;
static struct cdev *mydev;
static char *kbuf;
static int minor = 0;
static int inuse = 0;
static struct class *cl;
static int cln = 2000;
static int balance;

static int open (struct inode *inode, struct file *filp)
{
  inuse++;
  printk (KERN_INFO "This contain these many users %d\n", inuse);
  printk (KERN_INFO "MAJORNO %d and MINORNO %d\n", imajor (inode),iminor (inode));
  printk (KERN_INFO "This device process name %s and process id %d\n",current->comm, current->pid);
  printk (KERN_INFO "The refno is %d\n", module_refcount (THIS_MODULE));
  return 0;
}


static long char_dev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
  /*    step 1: verify args */

  unsigned int i,size;
  char *new_buf;
  char c;
  int retbytes;
  if (_IOC_TYPE(cmd) != SKG_MAGIC)
    return -ENOTTY;
  if (_IOC_NR(cmd) > SKG_MAX_CMDS)
    return -ENOTTY;

  if (_IOC_DIR(cmd) & _IOC_READ)
    if (!access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd)))
      return -EFAULT;
  if (_IOC_DIR(cmd) & _IOC_WRITE)
    if (!access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd)))
      return -EFAULT;

  /* implement support of commands using switch/case */

  /* sounds ok lets continue */
  switch(cmd)
    {

    case SKG_FILL_ZERO:
      			for (i = 0; i < MAX_SIZE; i++)
			kbuf[i] = 0;
      			printk(KERN_DEBUG " FILL_ZERO config executed\n ");
      			break;

    case SKG_FILL_CHAR:
      			retbytes = copy_from_user (&c, (char *)arg, sizeof(char));
      			for(i = 0; i < MAX_SIZE; i++)
			kbuf[i] = c;
      			printk(KERN_DEBUG "FILL_CHAR config executed \n");
      			break;

    case SKG_SET_SIZE:
      			if (!capable(CAP_SYS_ADMIN))
			return -EPERM;

      			retbytes = copy_from_user(&size,(unsigned int *)arg,sizeof(unsigned int));
      			new_buf = (char *) kmalloc(size * sizeof(char), GFP_KERNEL);
      			if (!new_buf)
			return -ENOSPC;
      			kfree(kbuf);
      			kbuf = (char *)new_buf;
      			MAX_SIZE = size;
      			for(i = 0; i < MAX_SIZE; i++)
			kbuf[i] = 0;
      			filp->f_pos = 0;
      			printk(KERN_DEBUG "SET_SIZE config executed\n");
      			break;

    case SKG_GET_SIZE:
      			if (!capable(CAP_SYS_ADMIN))
			return -EPERM;

      			size = MAX_SIZE;
      			retbytes = copy_to_user((unsigned int *)arg, &size, sizeof(unsigned int));
      			printk (KERN_DEBUG "GET_SIZE config executed \n");
      			break;

    }

  return 0;
}
static ssize_t read(struct file *file, char __user *buf, size_t ln, loff_t *pos)
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
		printk(KERN_INFO"This device reached the end\n");
		return -ENOSPC;
	}

	nbytes = bytes_to_do - copy_to_user(buf,kbuf+(*pos),bytes_to_do);
	*pos += nbytes;
	return nbytes;
}
static int close(struct inode *inode, struct file *filp)
{
  printk(KERN_INFO "This device is done the job\n");
  return 0;
}

static struct file_operations f_ops = {
	owner:THIS_MODULE,
  	open:open,
  	unlocked_ioctl:char_dev_ioctl,
  	read:read,
  	release:close,
};

static int __init start (void)
{
  if (alloc_chrdev_region (&first, minor, count, CHAR_NAME) < 0)
    {
      printk (KERN_INFO "This device is not register properly\n");
      return -1;
    }
  mydev = cdev_alloc ();
  if (!(mydev = cdev_alloc ()))
    {
      printk (KERN_INFO "This device is not alloated in the cdev\n");
      unregister_chrdev_region (first, count);
      return -1;
    }
  cdev_init (mydev, &f_ops);
  if (cdev_add (mydev, first, count) < 0)
    {
      printk (KERN_INFO "This driver is not allocated in the char_driver\n");
      cdev_del (mydev);
      unregister_chrdev_region (first, count);
      return -1;
    }
  cl = class_create (THIS_MODULE, "Myvirtual");
  device_create (cl, NULL, first, NULL, "%s", CHAR_NAME);
  printk (KERN_INFO"This device is register in the kernel directory with the  name %s\n",CHAR_NAME);
  printk (KERN_INFO"The process name %s and the process id %d of the device\n",current->comm, current->pid);
  printk (KERN_INFO "Majorno %d and the minorno %d\n", MAJOR (first),MINOR (first));
  kbuf = (char *) kzalloc (MAX_SIZE, GFP_KERNEL);
  return 0;
}

static void __exit stop(void)
{
  kfree (kbuf);
  device_destroy (cl, first);
  class_destroy (cl);
  cdev_del (mydev);
  unregister_chrdev_region (first, count);
  printk (KERN_INFO "This device is removed from the kernel\n");
}

module_init (start);
module_exit (stop);

MODULE_LICENSE ("Dual BSD/GPL");
