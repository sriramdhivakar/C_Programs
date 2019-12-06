CFLAGS_skel.o += -DDEBUG
obj-m += skel.o

KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
	@rm -v *~ cscope.*

