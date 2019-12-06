#ifndef _GPIO_COMMAND_H_
#define _GPIO_COMMAND_H_

#include <asm/ioctl.h>

#define magic 'A'

#define set_gpio_value		_IOW(magic,1,uint)
#define get_gpio_value		_IOR(magic,2,uint)

#define set_gpio_dir_in		_IO(magic,3)
#define get_gpio_dir_in		_IO(magic,4)

#define set_gpio_dir_out	_IOW(magic,5,uint)
#define get_gpio_dir_out	_IOW(magic,6,char)

#define set_gpio_pin		_IOW(magic,7,uint)
#define get_gpio_pin		_IOW(magic,8,uint)


#define set_gpio_unexp		_IOW(magic,9,uint)
#define get_gpio_unexp		_IOR(magic,10,uint)

#define set_gpio_free		_IOW(magic,11,uint)
#define get_gpio_free		_IOR(magic,12,uint)

#define max_gpio_cmds 12

#endif
