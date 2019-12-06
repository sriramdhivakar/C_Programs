#ifndef _GPIO_COMMAND_H_
#define _GPIO_COMMAND_H_

#include <asm/ioctl.h>

#define magic 'A'

#define set_gpio_high		_IO(magic,1)
#define set_gpio_low		_IO(magic,2)

#define max_gpio_cmds 2

#endif
