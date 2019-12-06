#include<linux/module.h>
#include<linux/init.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/kernel.h>      //����ʹ��ptintk����  
#include<linux/fs.h>
#include<linux/interrupt.h>   //���ж���ص�ͷ�ļ�  
#include<linux/cdev.h>           //�ַ��豸��ע��  
#include<linux/err.h>           //�����麯��  
#include<linux/slab.h>         //���ڴ�������  
#include<linux/sched.h>        //���ж���ص�ͷ�ļ�  
#include<asm/io.h>             //����IO�˿�  
#include<linux/wait.h>         //�ȴ�����   
#include<asm/uaccess.h>         //�û����ں˿ռ����ݵĿ���  
#include<linux/delay.h>          //ʱ���ӳٺ���
#include <linux/miscdevice.h>
#include<mach/irqs.h>
#include <linux/irq.h>

#include <asm/irq.h>

#define DEVICE_NAME "/dev/2440int"          //�����豸��   

#define GPGCON_ADDR  0x56000060   /*key*/
#define GPGDAT_ADDR  0x56000064

u32*             IMM32GPGCON;
u32*             IMM32GPGDAT;

static int eint2_value = 0;              //�ж�2��ֵ
static int eint_number = 0;              //�����жϵĴ���


static int TimerCount = 0;                   //��ʱ��־

#define TIMER_DELAY  (HZ/50)                 /* ������ ��ʱ:1��*/
#define TIMEOUT 3                            //��������ȥ����

static struct timer_list ring_timer;         //����һ��ʱ��

static  wait_queue_head_t wq ;               //�����ȴ�����

//spinlock_t repeat_lock;
spinlock_t lock;                             //����������


#define BLOCK   1                            //������ʽ
#define NONBLOCK    0                        //��������ʽ

static u_int FileMode = NONBLOCK;            //��ʼ��Ϊ������



static int requestIrq();                     //�����ж����뺯��


int get_eint2_value() {
    unsigned long IOValue;

    IOValue = *IMM32GPGDAT ;

    if (IOValue & 0x00000004) {                      //��eint2
        return 0;
    } else {
        return 1;
    }
}


static inline void  timer_handler(unsigned long data) { //��ʱ�жϷ���������ʱ����Σ���ֹ����
    if (!get_eint2_value()) {                      /* ���û�в����ⲿ�ж�2��˵���Ƕ�����������*/
        TimerCount = 0;                          //  ��0
        del_timer(&ring_timer);                  //ɾ����ʱ��
        local_irq_enable();                             //ʹ���ж�
    }

    TimerCount++;                                  //���ж�2����

    if (TimerCount == TIMEOUT) {                   //�����ǲ��ǵ����ζ�ʱ�����ж�2��ȷ�����а�������
        eint2_value = 1;                        //ȷ��һ���жϣ�����ֵ��Ϊ1��
        eint_number = eint_number + 1;          //�жϴ�����һ

        printk("int coming %d\n", eint_number); //��ӡ�ⲿ�жϴ���

        if (FileMode == BLOCK) {                //��ִ��
            wake_up_interruptible(&wq);    //���������ɱ��ж�
        }


        del_timer(&ring_timer);                 //ɾ����ʱ��
        TimerCount = 0;
        local_irq_enable();
    } else {
        ring_timer.expires = jiffies + TIMER_DELAY;  //���¸�ֵ��ʱ������ֵ
        add_timer(&ring_timer);                      //����������ʱ��
    }
}





//ʹ���ж�
static  void enableIrq() {
    //���SRCPND�Ĵ�����eint2��Ӧλ
    *IMM32GPGDAT = 0x00000004;

    //ʹ���ж�
    enable_irq(IRQ_EINT2);

}

static ssize_t
S3C2440_irint_read(struct file* filp, char* buffer, size_t count, loff_t* ppos) {

    //  printk("<0>,eint_number %d\n",eint_number);
    if (FileMode == BLOCK) {                    //��ִ��
        if (eint2_value == 0) {                 //���û������
            interruptible_sleep_on(&wq);        //һֱ˯�ߵȴ�

            //          if(signal_pending(current))
            //              return -ERESTARTSYS;
        }
    }

    count = sizeof(eint_number);
    copy_to_user(buffer, &eint_number, count);  //���������µ�ֵeint_number�������û��ռ�
    eint2_value = 0;                            //���ݶ�����ٴ����
    return count;

}

static int S3C2440_irint_open(struct inode* inode, struct file* filp) {
    return 0;
}

static int s3c2440_irint_release(struct inode* inode, struct file* filp) {
    disable_irq(IRQ_EINT2);                  //�ر��ж�2

    printk("release device\n");

    return 0;
}

static struct file_operations ring_fops = {
owner:
    THIS_MODULE,
open:
    S3C2440_irint_open,
read:
    S3C2440_irint_read,
release:
    s3c2440_irint_release,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &ring_fops ,
};


static void irint_interrupt(int irq, void* dev_id, struct pt_regs* regs)    //�ⲿ�ж�2�ķ�����

{

    spin_lock_irq(&lock);                                                   //������
    disable_irq(IRQ_EINT2);                                                 //�����ж�2

    //������ʱ��
    TimerCount = 0;
    ring_timer.expires = jiffies + TIMER_DELAY;                          //���ö�ʱʱ��
    add_timer(&ring_timer);                                              //������ʱ������ʱʱ��ҪԶԶ���ڶ���ʱ�䣬С�ڵڶ��ΰ��ж�2��ʱ�䣩

    //  enableIrq();
    spin_unlock_irq(&lock);                                                //����
}

static int requestIrq() {
    int ret;

    ret = request_irq(IRQ_EINT8, irint_interrupt, IRQ_TYPE_EDGE_BOTH,
                      DEVICE_NAME, NULL);                                         //ע���жϷ�����

    if (ret) {
        goto eint2_failed;
    }

    printk("requestIrq\n");
    return 0;


eint2_failed:
    free_irq(IRQ_EINT8, NULL);

eint_failed:

    printk(DEVICE_NAME ": IRQ Requeset Error\n");

    return ret;

}

static int __init s3c2440_irint_init(void) {
    int ret;

    misc_register(&misc);

    if (requestIrq() != 0) {
        misc_deregister(&misc);
        return -1;
    }

    IMM32GPGCON = (u32*) ioremap(GPGCON_ADDR, 4); /* key */
    IMM32GPGDAT = (u32*) ioremap(GPGDAT_ADDR, 4);


    init_timer(&ring_timer);                                                   //��ʼ����ʱ��
    ring_timer.function = timer_handler;                                       //���ö�ʱ�жϷ�����
    lock = SPIN_LOCK_UNLOCKED;                                                 //��ʼ��������
    init_waitqueue_head(&wq);                                                  //��ʼ���ȴ�����
    enableIrq();                                                               //ʹ���ж�



    return 0;
}

static void __exit s3c2440_irint_exit(void) {

    /*ע���豸*/
    misc_deregister(&misc);

    /*�ͷ��ж�*/
    free_irq(IRQ_EINT8, NULL);
}

module_init(s3c2440_irint_init);
module_exit(s3c2440_irint_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lxlong");
