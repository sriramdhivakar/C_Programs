#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/irq.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/interrupt.h>   //attentions
#include <linux/gpio.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>



static int times;
struct timer_list timer;

struct work_struct time_work;

unsigned long t = 0;

void time_work_funtion(unsigned long data) {

    //  while(flag%2);

    t = jiffies;

    while (time_after(jiffies, t + 15 * HZ) != 1);

    // sleep_on_timeout(&wait,2 * HZ);


    printk("work data :%d,result:%d\n", data);


    //tasklet_schedule(&time_tasklet);
}


void timer_funtion(unsigned long para) {
    printk("time:\n");

    //  schedule_work(&time_work);
    mod_timer(&timer, jiffies + (HZ));
}




int __init timer_init(void) {

    init_timer(&timer);
    timer.data = times;
    timer.expires = jiffies + HZ;
    timer.function = timer_funtion;
    INIT_WORK(&time_work, (void(*)(void*))time_work_funtion);
    //  add_timer(&timer);
    schedule_work(&time_work);
    // tasklet_schedule(&time_tasklet);
    // init_waitqueue_head(&wait);
    return 0;
}


void __exit timer_exit(void) {
    del_timer(&timer);

    printk("<1>takslet has been destroyed\n");
}

module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("lxlong");
MODULE_DESCRIPTION("tasklet test");
MODULE_ALIAS("wk module");
module_param(times, int, S_IRUGO);
MODULE_PARM_DESC(times, "timer set");


