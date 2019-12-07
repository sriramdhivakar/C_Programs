/************************************************************************\
 * file name 	: chrdrv_iomem_vidram.c					*
 * Description	: video RAM readming (Demo for IOMEM)			*
 *									*
\************************************************************************/


#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <asm/io.h>

#define VRAM_BASE 0x000A0000
//#define VRAM_SIZE 0x00000200
#define VRAM_SIZE 0x00000800
//#define VRAM_SIZE 0x000bffff

static void __iomem *skgram;
static dev_t mymajor;
static struct cdev sk_cdev;
static struct class *sk_class;

static int my_open(struct inode *i, struct file *f)
{
	return 0;
}

static int my_close(struct inode *i, struct file *f)
{
	return 0;
}

static ssize_t my_read(struct file *f, char __user * buf, size_t len,
		       loff_t * off)
{
	int i;
	u8 byte;

	pr_info("I am in read\n");
	if (*off >= VRAM_SIZE) {
		return 0;
	}
	if (*off + len > VRAM_SIZE) {
		len = VRAM_SIZE - *off;
	}
	for (i = 0; i < len; i++) {
		byte = ioread8((u8 *) skgram + *off + i);
		if (copy_to_user(buf + i, &byte, 1)) {
			return -EFAULT;
		}
	}
	*off += len;
	pr_info("I am in read\n");

	return len;
}

static ssize_t my_write(struct file *f, const char __user * buf,
			size_t len, loff_t * off)
{
	int i;
	u8 byte;

	pr_info("I am in iwrite\n");
	if (*off >= VRAM_SIZE) {
		return 0;
	}
	if (*off + len > VRAM_SIZE) {
		len = VRAM_SIZE - *off;
	}
	for (i = 0; i < len; i++) {
		if (copy_from_user(&byte, buf + i, 1)) {
			return -EFAULT;
		}
		printk("%c", byte);
		iowrite8(byte, (u8 *) skgram + *off + i);
	}
	*off += len;
	pr_info("I am in write\n");

	return len;
}

static struct file_operations skgram_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.read = my_read,
	.write = my_write
};

static int __init skgram_init(void)
{				/* Constructor */
	if ((skgram = ioremap(VRAM_BASE, VRAM_SIZE)) == NULL) {
		printk(KERN_ERR "Mapping video RAM failed\n");
		return -1;
	}

	pr_info("IOREMAP VRAM address : %p \n", skgram);
	if (alloc_chrdev_region(&mymajor, 0, 1, "skgram") < 0) {
		return -1;
	}
	cdev_init(&sk_cdev, &skgram_fops);
	if (cdev_add(&sk_cdev, mymajor, 1) == -1) {
		device_destroy(sk_class, mymajor);
		class_destroy(sk_class);
		unregister_chrdev_region(mymajor, 1);
		return -1;
	}

	if ((sk_class = class_create(THIS_MODULE, "skgram")) == NULL) {
		unregister_chrdev_region(mymajor, 1);
		return -1;
	}
	if (device_create(sk_class, NULL, mymajor, NULL, "skgram") == NULL) {
		class_destroy(sk_class);
		unregister_chrdev_region(mymajor, 1);
		return -1;
	}

	pr_info("Device created\n");
	return 0;
}

static void __exit skgram_exit(void)
{
	cdev_del(&sk_cdev);
	device_destroy(sk_class, mymajor);
	class_destroy(sk_class);
	unregister_chrdev_region(mymajor, 1);
	iounmap(skgram);
}

module_init(skgram_init);
module_exit(skgram_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Video RAM Driver");
