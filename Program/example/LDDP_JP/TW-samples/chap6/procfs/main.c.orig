#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("Dual BSD/GPL");

#define PROCNAME "driver/sample"

#define DUMMY_BUFSIZ 4096
static char dummy[DUMMY_BUFSIZ];

/*執行cat /proc/xxxx時,什麼時候調用read_proc,什麼時候調用proc_fops->read呢?
 * 答案:
 * 在註冊entry的時候，如果提供read_proc,讀文件時調用路徑為
 * proc_file_operations->read->proc_file_read->read_proc
 * */

static int sample_read_proc(char *page, char **start, off_t off, 
		int count, int *eof, void *data)
{
	int len;

    dump_stack();

	printk("page=%p *start=%p off=%d count=%d *eof=%d data=%p\n",
			page, *start, (int)off, count, *eof, data);

	len = DUMMY_BUFSIZ;
	if (len > PAGE_SIZE)
		return -ENOBUFS;

	memcpy(page, dummy, len);  // dump 內容ABC~Z, totoal 4096byte 存放到page , user space 在讀取page , len = 4096
	*eof = 1;

	printk("len=%d (%s)\n", len, __func__);
	return (len);
}

static int sample_init(void)
{
	struct proc_dir_entry *entry;
	int i;

	/* add /proc */
	entry = create_proc_read_entry(PROCNAME, S_IRUGO | S_IWUGO, NULL, sample_read_proc, NULL);
	if (entry == NULL) {
		printk(KERN_WARNING "sample: unable to create /proc entry\n");
	}

	for (i = 0 ; i < DUMMY_BUFSIZ ; i++) {
		dummy[i] = 'A' + (i % 26);
	}

	printk("driver loaded\n");
		
	return 0;
}

static void sample_exit(void)
{
	remove_proc_entry(PROCNAME, NULL);

	printk(KERN_ALERT "driver unloaded\n");
}

module_init(sample_init);
module_exit(sample_exit);

