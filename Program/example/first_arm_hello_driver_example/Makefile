#hello_makefile ARM
obj-m :=hello.o
KRNELDIR := /media/shihyu/toshiba/android_42_pure/goldfish
CROSS_COMPILE = arm-eabi-
CC :=$(CROSS_COMPILE)gcc
LD :=$(CROSS_COMPILE)ld
PWD :=$(shell pwd)
all:
	make -C $(KRNELDIR) M=$(PWD) modules ARCH=arm 
	
.PHONY :clean
clean:
	rm -rf *.o *ko
