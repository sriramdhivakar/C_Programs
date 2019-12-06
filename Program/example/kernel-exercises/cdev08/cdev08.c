/**
 * file: cdev08.c
 * desc: fifo buffers of PAGE_SIZE bytes.
 *       reads will block data is avaible
 *       writes will wake up reader processes
 * compile: just use make and change the KDIR path on Makefile
 * load: insmod cdev08 [ndevs=N]
 *       defautl is ndevs = 6
 * author: Daniel Hilst Selli <daniel.hilst at gmail.com>
 */

#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/wait.h>

#define MODULE_NAME "cdev08.c"
#define DEBUG(msg, args...) do {                                         \
                printk(KERN_DEBUG MODULE_NAME " %s()@%d " msg, __func__, __LINE__, ##args); \
                } while (0)

#define DEVICE_NAME "gdev"
#define FIRST 0

static int ndevs = 6;
module_param(ndevs, int, 0600);
MODULE_PARM_DESC(ndevs,"Number of buffers to create");

struct gdev_s {
        dev_t dev;
        struct cdev *cdev;
        struct class *class;
        struct device **device;
};

struct gdev_buffer_s {
        wait_queue_head_t wait;
        char *buffer;
        atomic_t used;
};

/* array, is size determined at load time
 * by @ndevs module param
 */
static struct gdev_buffer_s *gbuffer;

static struct gdev_s gdev;

static int dev_open(struct inode *ip, struct file *fp)
{
        return 0;
}

static int dev_release(struct inode *ip, struct file *fp)
{
        return 0;
}


#define TRACE_RW do { \
                DEBUG("called with len = %d, *off = %d", (int)len, (int)*off); \
        } while (0) 


static ssize_t dev_read(struct file *fp,
                        char *buf,
                        size_t len,
                        loff_t *off)
{
        int tocopy;
        int copied;
        int i = MINOR(fp->f_inode->i_rdev) - FIRST;
        char *hello = gbuffer[i].buffer;
        
        TRACE_RW;

        /* block */
        do {
                int n = wait_event_interruptible(gbuffer[i].wait, 
                        *off < atomic_read(&gbuffer[i].used));

                if (n)
                        return -ERESTARTSYS;

        } while (0);

        DEBUG("waked up!");
        DEBUG("len = %d, *off = %d, used = %d", (int)len, (int)*off,
              (int)atomic_read(&gbuffer[i].used));
        /* len is never 0
         * because atomic_read(&gbuffer[i].used) >= *off here
         */
        if (len + *off > atomic_read(&gbuffer[i].used)) 
                len = atomic_read(&gbuffer[i].used) - *off;

        tocopy = copy_to_user(buf,
                              hello + *off,
                              len);

        copied = len - tocopy;
        *off += copied;
        return copied;
}

static ssize_t dev_write(struct file *fp,
                         const char *buf,
                         size_t len,
                         loff_t *off)
{
        int tocopy;
        int copied;
        int i = MINOR(fp->f_inode->i_rdev) - FIRST;
        int free;

        TRACE_RW;

        DEBUG("minor is index is %d", i);

        free = PAGE_SIZE - atomic_read(&gbuffer[i].used);
        DEBUG("free bytes = %d", (int) free);
        if (len > free) {
                len = free;
                DEBUG("no space left, trucated to %d bytes", (int)len);
        }

        tocopy = copy_from_user(
                gbuffer[i].buffer + atomic_read(&gbuffer[i].used),
                buf,
                len);

        copied = len - tocopy;
        atomic_add(copied, &gbuffer[i].used);
        wake_up_interruptible(&gbuffer[i].wait);
        *off += copied;
        DEBUG("buffer length is %d", atomic_read(&gbuffer[i].used));
        DEBUG("writed %d bytes", copied);
        return copied;
}
        

struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = dev_read,
        .write = dev_write,
        .open = dev_open,
        .release = dev_release,
};

int init_module(void)
{
        int error;
        int rval;
        int i = 0;
       
        DEBUG("module initialize");

        /* @ndevs number of devices is an module argument */
        gbuffer = kmalloc(sizeof (struct gdev_buffer_s) * ndevs, GFP_KERNEL);
        if (!gbuffer) {
                DEBUG("kmalloc() fails");
                return -1;
        }

        error = alloc_chrdev_region(&gdev.dev, FIRST, ndevs, DEVICE_NAME);
        if (error) {
                DEBUG("alloc_chrdev_region() fails");
                goto alloc_chrdev_region_error;
        }

        gdev.class = class_create(THIS_MODULE, "gko");
        if (IS_ERR(gdev.class)) {
                DEBUG("class_create() fails");
                goto class_error;
        }

        gdev.cdev = cdev_alloc();
        if (!gdev.cdev) {
                DEBUG("alloc_cdev() fails at %i", i);
                goto cdev_error;
        }

        cdev_init(gdev.cdev, &fops);

        rval = cdev_add(gdev.cdev, gdev.dev, ndevs);
        if (rval < 0) {
                DEBUG("cdev_add() fails");
                goto cdev_error;
        }

        gdev.device = kmalloc(sizeof (struct device *) * ndevs, GFP_KERNEL);
        if (!gdev.device) {
                DEBUG("kamalloc() fails");
                goto cdev_error;
        }

        DEBUG("ndevs = %d", ndevs);
        for (i = 0; i < ndevs; i++) {
                int minor = MINOR(gdev.dev) + i;
                int major = MAJOR(gdev.dev);

                gdev.device[i] = device_create(gdev.class, NULL, 
                                            MKDEV(major, minor), 
                                            NULL, "gdev%d", minor);

                if (IS_ERR(gdev.device[i])) {
                        DEBUG("device_create() fails");
                        goto device_error;
                } else 
                        DEBUG("device gdev%d created", minor);

                /**
                 * gbuffer initialization
                 */
                gbuffer[i].buffer = (char *) __get_free_page(GFP_KERNEL);
                if (!gbuffer[i].buffer) {
                        DEBUG("__get_free_page() fails");
                }
                atomic_set(&gbuffer[i].used, 0);
                init_waitqueue_head(&gbuffer[i].wait);
        }

        return 0;

device_error:
        do {
                int j;
                for (j = 0; j < i; j++) {
                        device_destroy(gdev.class, MKDEV(MAJOR(gdev.dev),
                                                         MINOR(gdev.dev) + j));
                        free_page((unsigned long)gbuffer[i].buffer);
                }
        } while (0);
        kfree(gdev.device);

cdev_error:
        cdev_del(gdev.cdev);
        unregister_chrdev_region(gdev.dev, ndevs);

class_error:
        class_unregister(gdev.class);
        class_destroy(gdev.class);

alloc_chrdev_region_error:
        kfree(gbuffer);

        return -1;
}

void cleanup_module(void)
{
        int i;
        for (i = 0; i < ndevs; i++) {
                device_destroy(gdev.class, MKDEV(MAJOR(gdev.dev),
                                                 MINOR(gdev.dev) + i));
                free_page((unsigned long)gbuffer[i].buffer);
        }

        class_unregister(gdev.class);
        class_destroy(gdev.class);
        cdev_del(gdev.cdev);
        unregister_chrdev_region(gdev.dev, ndevs);
        kfree(gbuffer);
}


MODULE_LICENSE("GPL");
