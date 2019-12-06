#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/gpio.h>

#define DEVICE_NAME "inters" //�豸����


/*�����ж����õĽṹ��*/

struct button_irq_desc

{
    int irq; //������Ӧ���жϺ�
    int pin; //��������Ӧ��GPIO �˿�
    int pin_setting; //������Ӧ������������ʵ�ʲ�δ�õ�������
    int number; //�����ֵ���Դ��ݸ�Ӧ�ò�/�û�̬
    char* name; //ÿ������������
};

/*�ṹ��ʵ�嶨��*/
static struct button_irq_desc button_irqs [] = {
    {IRQ_EINT8 , S3C2410_GPG(0) , S3C2410_GPG0_EINT8 , 0, "KEY0"},
    {IRQ_EINT11, S3C2410_GPG(3) , S3C2410_GPG3_EINT11 , 1, "KEY1"},
    {IRQ_EINT13, S3C2410_GPG(5) , S3C2410_GPG5_EINT13 , 2, "KEY2"},
    {IRQ_EINT14, S3C2410_GPG(6) , S3C2410_GPG6_EINT14 , 3, "KEY3"},
    {IRQ_EINT15, S3C2410_GPG(7) , S3C2410_GPG7_EINT15 , 4, "KEY4"},
    {IRQ_EINT19, S3C2410_GPG(11), S3C2410_GPG11_EINT19, 5, "KEY5"},
};

/*�������ϰ�����״̬������ע�������ǡ�0������Ӧ��ASCII ��Ϊ30*/
static volatile char key_values [] = {'0', '0', '0', '0', '0', '0'};
/*��Ϊ�������ǻ����жϷ�ʽ�ģ��ڴ˴���һ���ȴ����У�������жϺ���ʹ�ã����а������²���ȡ����ֵʱ�����ỽ�Ѵ˶��У��������жϱ�־���Ա���ͨ�� read �����жϺͶ�ȡ��ֵ���ݵ��û�̬����û�а�������ʱ��ϵͳ����*/
/*����ѯ����״̬���Խ�ʡʱ����Դ*/
static DECLARE_WAIT_QUEUE_HEAD(button_waitq);
/*�жϱ�ʶ�������������Ķ���ʹ�ã��жϷ��������������Ϊ1��read �������������*/
static volatile int ev_press = 0;
/*�������������жϷ������*/
static irqreturn_t buttons_interrupt(int irq, void* dev_id) {
    struct button_irq_desc* button_irqs = (struct button_irq_desc*)dev_id;
    int down;
    // udelay(0);
    /*��ȡ�����µİ���״̬*/
    down = !s3c2410_gpio_getpin(button_irqs->pin);

    /*״̬�ı䣬���������£��������Կ�����������û�б����µ�ʱ�򣬼Ĵ�����ֵΪ1(����)�������������µ�ʱ�򣬼Ĵ�����Ӧ��ֵΪ0*/

    if (down != (key_values[button_irqs->number] & 1)) { // Changed
        /*���key1 �����£���key_value[0]�ͱ�Ϊ��1������Ӧ��ASCII ��Ϊ31*/
        key_values[button_irqs->number] = '0' + down;
        ev_press = 1; /*�����жϱ�־Ϊ1*/
        wake_up_interruptible(&button_waitq); /*���ѵȴ�����*/

    }

    return IRQ_RETVAL(IRQ_HANDLED);
}

/**��Ӧ�ó���ִ��open(��/dev/buttons��,��)ʱ����õ��˺��������������������Ҫ��ע��6 ���������жϡ�

*���õ��ж�������IRQ_TYPE_EDGE_BOTH��Ҳ����˫�ش������������غ��½��ؾ�������жϣ�������

**��Ϊ�˸�����Ч���жϰ���״̬

*/
static int s3c24xx_buttons_open(struct inode* inode, struct file* file) {
    int i;
    int err = 0;

    for (i = 0; i < sizeof(button_irqs) / sizeof(button_irqs[0]); i++) {
        if (button_irqs[i].irq < 0) {
            continue;
        }

        /*ע���жϺ���*/
        err = request_irq(button_irqs[i].irq, buttons_interrupt, IRQ_TYPE_EDGE_BOTH,
                          button_irqs[i].name, (void*)&button_irqs[i]);

        if (err) {
            break;
        }
    }

    if (err) {

        /*��������ͷ��Ѿ�ע����жϣ�������*/

        i--;

        for (; i >= 0; i--) {
            if (button_irqs[i].irq < 0) {
                continue;
            }

            disable_irq(button_irqs[i].irq);
            free_irq(button_irqs[i].irq, (void*)&button_irqs[i]);
        }

        return -EBUSY;
    }

    /*ע��ɹ������ж϶��б��Ϊ1����ʾ����ͨ��read ��ȡ*/
    ev_press = 1;
    /*��������*/
    return 0;
}
/*

*�˺�����ӦӦ�ó����ϵͳ����close(fd)�������ڴˣ�������Ҫ�����ǵ��ر��豸ʱ�ͷ�6 ���������ж�*������

*/

static int s3c24xx_buttons_close(struct inode* inode, struct file* file) {
    int i;

    for (i = 0; i < sizeof(button_irqs) / sizeof(button_irqs[0]); i++) {
        if (button_irqs[i].irq < 0) {
            continue;
        }

        /*�ͷ��жϺţ���ע���жϴ�����*/
        free_irq(button_irqs[i].irq, (void*)&button_irqs[i]);
    }

    return 0;
}

/*

*��ӦӦ�ó����read(fd,��)��������Ҫ�������û��ռ䴫�ݼ�ֵ

*/

static int s3c24xx_buttons_read(struct file* filp, char __user* buff, size_t count, loff_t* offp) {
    unsigned long err;

    if (!ev_press) {
        if (filp->f_flags & O_NONBLOCK)

            /*���жϱ�ʶΪ0 ʱ�����Ҹ��豸���Է�������ʽ��ʱ������*/
        {
            return -EAGAIN;
        } else

            /*���жϱ�ʶΪ0 ʱ�����Ҹ��豸����������ʽ��ʱ����������״̬���ȴ�������*/
        {
            wait_event_interruptible(button_waitq, ev_press);
        }
    }

    /*���жϱ�ʶ����*/
    ev_press = 0;
    /*һ���ֵ�����ݵ��û��ռ�*/
    err = copy_to_user(buff, (const void*)key_values, min(sizeof(key_values), count));

    return err ? -EFAULT : min(sizeof(key_values), count);
}

static unsigned int s3c24xx_buttons_poll(struct file* file, struct poll_table_struct* wait) {
    unsigned int mask = 0;
    /*�ѵ���poll ����select �Ľ��̹�����У��Ա㱻����������*/
    poll_wait(file, &button_waitq, wait);

    if (ev_press) {
        mask |= POLLIN | POLLRDNORM;
    }

    return mask;
}

/*�豸������*/

static struct file_operations dev_fops = {
    .owner = THIS_MODULE,
    .open = s3c24xx_buttons_open,
    .release = s3c24xx_buttons_close,
    .read = s3c24xx_buttons_read,
    .poll = s3c24xx_buttons_poll,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &dev_fops,
};

/*�豸��ʼ������Ҫ��ע���豸*/
static int __init dev_init(void) {
    int ret;
    /*�Ѱ����豸ע��Ϊmisc �豸�����豸�����Զ������*/
    ret = misc_register(&misc);
    printk(DEVICE_NAME"\tinitialized\n");
    return ret;
}

/*ע���豸*/
static void __exit dev_exit(void) {
    misc_deregister(&misc);
}

module_init(dev_init); //ģ���ʼ��������ʹ��insmod/podprobe �������ʱ���ã�����豸����ͨ��ģ�鷽ʽ���أ��˴������ᱻ����
module_exit(dev_exit); //ж��ģ�飬�����豸ͨ��ģ�鷽ʽ���غ󣬿���ͨ��rmmod ����ж�أ������ô˺���
MODULE_LICENSE("GPL");//��Ȩ��Ϣ

