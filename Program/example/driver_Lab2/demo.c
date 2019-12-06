/*****************************************************************************/
#include <linux/ioctl.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

struct ioctl_arg {
    unsigned int reg;
    unsigned int val;
};

/* 這裡要找一個沒用到的號碼，請參考 Documentation/ioctl/ioctl-number.txt */
#define IOC_MAGIC '\x66'

/* 您要的動作 */
#define IOCTL_VALSET      _IOW(IOC_MAGIC, 0, struct ioctl_arg)
#define IOCTL_VALGET      _IOR(IOC_MAGIC, 1, struct ioctl_arg)
#define IOCTL_VALGET_NUM  _IOR(IOC_MAGIC, 2, int)
#define IOCTL_VALSET_NUM  _IOW(IOC_MAGIC, 3, int)

#define IOCTL_VAL_MAXNR 3
#define DRIVER_NAME "test_ioctl"
static unsigned int test_ioctl_major = 0;
static unsigned int num_of_dev = 1;
static struct cdev test_ioctl_cdev;
static int ioctl_num = 0;

struct test_ioctl_data {
    unsigned char val;
    rwlock_t lock;
};

static long test_ioctl_ioctl(struct file* filp, unsigned int cmd, unsigned long arg) {
    struct test_ioctl_data* ioctl_data = filp->private_data;
    int retval;
    unsigned char val;
    struct ioctl_arg data;
    memset(&data, 0, sizeof(data));

    switch (cmd) {
    case IOCTL_VALSET:

        /*
        if (!capable(CAP_SYS_ADMIN)) {
         retval = -EPERM;
         goto done;
        }
        if (!access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd))) {
         retval = -EFAULT;
         goto done;
        }
        */
        if (copy_from_user(&data, (int __user*)arg, sizeof(data))) {
            retval = -EFAULT;
            goto done;
        }

        printk(KERN_ALERT "IOCTL set val:%x .\n", data.val);
        write_lock(&ioctl_data->lock);
        ioctl_data->val = data.val;
        write_unlock(&ioctl_data->lock);
        break;

    case IOCTL_VALGET:
        /*
        if (!access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd))) {
                                     retval = -EFAULT;
                                     goto done;
                             }
        */
        read_lock(&ioctl_data->lock);
        val = ioctl_data->val;
        read_unlock(&ioctl_data->lock);
        data.val = val;

        if (copy_to_user((int __user*)arg, &data, sizeof(data))) {
            retval = -EFAULT;
            goto done;
        }

        break;

    case IOCTL_VALGET_NUM:
        retval = __put_user(ioctl_num, (int __user*)arg);
        break;

    case IOCTL_VALSET_NUM:
        /*
        if (!capable(CAP_SYS_ADMIN))
         return -EPERM;
        */
        ioctl_num = arg;
        break;

    default:
        retval = -ENOTTY;
    }

done:
    return retval;
}

ssize_t test_ioctl_read(struct file* filp, char __user* buf, size_t count, loff_t* f_pos) {
    struct test_ioctl_data* ioctl_data = filp->private_data;
    unsigned char val;
    int retval;
    int i = 0;
    read_lock(&ioctl_data->lock);
    val = ioctl_data->val;
    read_unlock(&ioctl_data->lock);

    for (; i < count ; i++) {
        if (copy_to_user(&buf[i], &val, 1)) {
            retval = -EFAULT;
            goto out;
        }
    }

    retval = count;
out:
    return retval;
}

static int test_ioctl_close(struct inode* inode, struct file* filp) {
    printk(KERN_ALERT "%s call.\n", __func__);

    if (filp->private_data) {
        kfree(filp->private_data);
        filp->private_data = NULL;
    }

    return 0;
}

static int test_ioctl_open(struct inode* inode, struct file* filp) {
    struct test_ioctl_data* ioctl_data;
    printk(KERN_ALERT "%s call.\n", __func__);
    ioctl_data = kmalloc(sizeof(struct test_ioctl_data), GFP_KERNEL);

    if (ioctl_data == NULL) {
        return -ENOMEM;
    }

    rwlock_init(&ioctl_data->lock);
    ioctl_data->val = 0xFF;
    filp->private_data = ioctl_data;
    return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = test_ioctl_open,
    .release = test_ioctl_close,
    .read = test_ioctl_read,
    .unlocked_ioctl = test_ioctl_ioctl,
};

static int test_ioctl_init(void) {
    dev_t dev = MKDEV(test_ioctl_major, 0);
    int alloc_ret = 0;
    int cdev_ret = 0;
    alloc_ret = alloc_chrdev_region(&dev, 0, num_of_dev, DRIVER_NAME);

    if (alloc_ret) {
        goto error;
    }

    test_ioctl_major = MAJOR(dev);
    cdev_init(&test_ioctl_cdev, &fops);
    cdev_ret = cdev_add(&test_ioctl_cdev, dev, num_of_dev);

    if (cdev_ret) {
        goto error;
    }

    printk(KERN_ALERT "%s driver(major: %d) installed.\n", DRIVER_NAME, test_ioctl_major);
    return 0;
error:

    if (cdev_ret == 0) {
        cdev_del(&test_ioctl_cdev);
    }

    if (alloc_ret == 0) {
        unregister_chrdev_region(dev, num_of_dev);
    }

    return -1;
}

static void test_ioctl_exit(void) {
    dev_t dev = MKDEV(test_ioctl_major, 0);
    cdev_del(&test_ioctl_cdev);
    unregister_chrdev_region(dev, num_of_dev);
    printk(KERN_ALERT "%s driver removed.\n", DRIVER_NAME);
}

module_init(test_ioctl_init);
module_exit(test_ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wang Chen Shu");
MODULE_DESCRIPTION("This is test_ioctl module.");
