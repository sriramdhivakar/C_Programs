CROSS_COMPILE	?= ppc_6xx-

CC = $(CROSS_COMPILE)gcc

obj-m := myNotifier.o

KERNELDIR = /usr/src/linux-2.6.24
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=powerpc CROSS_COMPILE=ppc_6xx- modules
	chmod 777 myNotifier.ko
	cp myNotifier.ko /media/disk/lxl
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=powerpc clean
	rm -f *.ko *.mod *.o *.symvers *~ *.order


