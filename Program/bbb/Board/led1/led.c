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
static unsigned int gpio;

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

	int nbytes;
	int pbuf,y;
	unsigned int n;
	switch(cmd)
	{	
		case set_gpio_value:
			nbytes = copy_from_user(&n,(int *)arg,sizeof(int));
			gpio = n;
			printk(KERN_INFO"The value of gpio is %d\n",gpio);
			if(!(gpio_is_valid(gpio)))
			{
				printk(KERN_ERR"This gpio is not valid\n");
				return -ENODEV;
			}
			printk(KERN_INFO"The gpio%d is valid\n",gpio);
			gpio_request(gpio,"sysfs");
			gpio_export(gpio,false);//dought
			printk(KERN_INFO"The gpio%d is exported\n",gpio);
			break;

		case get_gpio_value:
			nbytes = copy_to_user((int *)arg,&gpio,sizeof(int));
			break;

		case set_gpio_dir_out:
			nbytes = copy_from_user(&n,(int *)arg,sizeof(int));
			pbuf = n;
			printk(KERN_INFO"The gpio_dir is %s\n","out");
			if(pbuf == 0){
				gpio_direction_output(gpio,false);
			}
			else{
				gpio_direction_output(gpio,true);
			}
			break;
	
		case get_gpio_dir_out:
			y = 0;
			nbytes = copy_to_user((char *)arg,&y,sizeof(char));
			break;
		/*case set_gpio_dir_in:
			nbytes = copy_from_user(&n,(int *)arg,sizeof(int));
			pbuf = n;
			printk(KERN_INFO"The gpio_dir is %s\n","out");
			gpio_direction_input(pbuf);//'0'=dought
			break;*/
			

		case set_gpio_pin:
			nbytes = copy_from_user(&n,(int *)arg,sizeof(int));
			pbuf = n;
			printk(KERN_INFO"The value of n is %d\n",pbuf);//change
			gpio_set_value(gpio,pbuf);
			break;

		case get_gpio_pin:
			pbuf = gpio_get_value(gpio);
			nbytes = copy_to_user((unsigned int *)arg,&pbuf,sizeof(unsigned int));
			break;
		
		case set_gpio_unexp:
			nbytes = copy_from_user(&n,(int *)arg,sizeof(int));
			gpio = n;
			gpio_unexport(gpio);
			printk(KERN_INFO"gpio%d is unexported\n",gpio);
			break;

		case get_gpio_unexp:
			y = 0;
			nbytes = copy_to_user((char *)arg,&y,sizeof(char));
			break;

		case set_gpio_free:
			nbytes = copy_from_user(&n,(int *)arg,sizeof(int));
			gpio = n;
			gpio_free(gpio);
			printk(KERN_INFO"gpio%d is free\n",gpio);
			break;

		case get_gpio_free:
			y = 0;
			nbytes = copy_to_user((char *)arg,&y,sizeof(char));
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
	return 0;
}
static void __exit stop (void)
{
	gpio_set_value(gpio,0);
	gpio_unexport(gpio);
	gpio_free(gpio);
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
MODULE_AUTHOR("sriram");
