CFLAGS_skel.o += -DDEBUG
obj-m += hello-world.o

KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
	@rm -v *~ cscope.*

