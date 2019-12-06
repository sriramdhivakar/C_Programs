#ifndef _SKG_CHAR_DEVICE_H_
#define _SKG_CHAR_DEVICE_H_

#include <asm/ioctl.h>

#define SKG_MAGIC 'A'

#define SKG_FILL_ZERO        _IO( SKG_MAGIC, 1 )

#define SKG_FILL_CHAR        _IOW( SKG_MAGIC, 2, char )


#define SKG_SET_SIZE        _IOW( SKG_MAGIC, 3, uint )

#define SKG_GET_SIZE        _IOR( SKG_MAGIC, 4, uint )

#define SKG_MAX_CMDS        4

#endif /* _SKG_CHAR_DEVICE_H_ */
