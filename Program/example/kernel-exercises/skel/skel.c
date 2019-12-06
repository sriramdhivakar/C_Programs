/**
 * @file CHANGE HERE
 * @author Daniel Hilst Selli <danielhilst@gmail.com>
 * @brief CHANGE HERE
 */

#include <linux/module.h>
#include <linux/fs.h>

#define MODULE_NAME "CHANGE HERE"
#define _DEBUG(fmt, msg...)                                             \
        do {                                                            \
                pr_debug("%s:%s@%d: "                                   \
                         fmt, MODULE_NAME, __func__, __LINE__, ##msg);  \
        } while (0);


static int dev_open(struct inode *ip, struct file *fp)
{
        return 0;
}

static int dev_release(struct inode *ip, struct file *fp)
{
        return 0;
}

static ssize_t dev_write(struct file *fp,
                         const char *buf,
                         size_t len,
                         loff_t *off)
{
        _DEBUG("");
        return 0;
}

static ssize_t dev_read(struct file *fp,
                        char *buf,
                        size_t len,
                        loff_t *off)
{
        _DEBUG("");
        return 0;
}

struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = dev_read,
        .write = dev_write,
        .open = dev_open,
        .release = dev_release,
};

static int __init dev_init(void)
{
        _DEBUG("");
        return 0;
}

static void __exit dev_exit(void)
{
        _DEBUG("");
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_AUTHOR("Daniel Hilst Selli <danielhilst@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CHANGE HERE");
