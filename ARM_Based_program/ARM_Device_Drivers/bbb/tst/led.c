#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/current.h>
#include <asm/uaccess.h>
#include "gpio_command.h"

#define CHAR_NAME "gpio"
#define MAX_SIZE 2000


static dev_t first;
static int minor = 0;
static int count = 1;
static struct cdev *mydev;
static struct class *cls;
static char *kbuf;
static int inuse = 0;
static unsigned int gpioled = 49;

static int open(struct inode *inode, struct file *filp)
{
  inuse++;
  printk(KERN_INFO "This device has these many user %d\n", inuse);
  printk(KERN_INFO "The Majorno %d and Minorno %d\n", imajor (inode),iminor (inode));
  printk(KERN_INFO "The process name %s and process id %d\n", current->comm,current->pid);
  printk(KERN_INFO "The refno = %ld\n", module_refcount (THIS_MODULE));

  return 0;
}

static long ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

	switch(cmd)
	{
		case set_gpio_high:
			gpio_set_value(gpioled,0);
			break;
		case set_gpio_low:
			gpio_set_value(gpioled,1);
			break;
	}
	return 0;	
}
static int close(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO"This device is done the job\n");
	return 0;
}	
static struct file_operations f_ops ={
	.owner = THIS_MODULE,
	.open = open,
	.unlocked_ioctl = ioctl,
	.release =  close,
}; 
static int __init start (void)
{
	if(alloc_chrdev_region (&first, minor, count, CHAR_NAME) < 0)
        {
		 printk (KERN_ERR"This device is not register in the alloc in fs.h\n");
	  	return -1;
	}

	if (!(mydev = cdev_alloc ()))
	{
	  	printk (KERN_ERR "This device is not add to cdev\n");
		unregister_chrdev_region (first, count); 
		return -1;
	}

	cdev_init (mydev, &f_ops); 
	if (cdev_add (mydev, first, count) < 0)
	{
	  	printk (KERN_ERR "This device is not added to the cdev\n");
	  	cdev_del (mydev);
	  	unregister_chrdev_region (first, count); 
		return -1;
	}

	cls = class_create (THIS_MODULE, "Myvirtual");
	device_create (cls, NULL, first, NULL, "%s", CHAR_NAME);
	kbuf = (char *) kzalloc (MAX_SIZE, GFP_KERNEL);
	printk(KERN_INFO "This device is create with the name of %s\n",CHAR_NAME);
	printk(KERN_INFO "This device process name %sand process id %d\n", current->comm, current->pid);
	printk(KERN_INFO "The Majorno %d and Minorno %d\n", MAJOR (first),MINOR (first)); 
  	printk(KERN_INFO "GPIO_TEST: Initalization the GPIO_TEST LKM\n");
	if(!gpio_is_valid(gpioled))
	{
		printk(KERN_INFO"GPIO_TEST: invalid LED GPIO\n");
		return -ENODEV;
	}
	
	gpio_request(gpioled,"sysfs");
	gpio_export(gpioled,false);
	gpio_direction_output(gpioled,0);
	return 0;
}
static void __exit stop (void)
{
	gpio_set_value(gpioled,0);
	gpio_unexport(gpioled);
	gpio_free(gpioled);
	kfree (kbuf);
	device_destroy (cls, first);
	class_destroy (cls);
	cdev_del (mydev);
	unregister_chrdev_region (first, count);
	printk (KERN_INFO"This device is successfully removed from the kernel\n");
}

module_init (start);
module_exit (stop);
	  
MODULE_AUTHOR ("sriram");
MODULE_LICENSE ("Dual BSD/GPL"); 
MODULE_VERSION ("b.0.2");
/*static unsigned int gpioled = 49;
static bool ledon = 0;

static int __init start(void)
{
	printk(KERN_INFO"GPIO_TEST: Initalization the GPIO_TEST LKM\n");
	
	if(!gpio_is_valid(gpioled))
	{
		printk(KERN_INFO"GPIO_TEST: invalid LED GPIO\n");
		return -ENODEV;
	}

	ledon = true;
	gpio_request(gpioled,"sysfs");
	gpio_export(gpioled,false);
	gpio_direction_output(gpioled,ledon);
	gpio_set_value(gpioled,ledon);

	printk(KERN_INFO"Led current status : %d\n",ledon);
	return 0;
}

static void __exit stop(void)
{
	gpio_set_value(gpioled,0);
	gpio_unexport(gpioled);
	gpio_free(gpioled);
	printk(KERN_INFO"GPIO_TEST: Goodbye from the LKM\n");
}

module_init(start);
module_exit(stop);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("bv0.1");
MODULE_AUTHOR("sriram");*/
