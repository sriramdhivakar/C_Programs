obj-m += cdev07.o

KDIR = /home/geckos/abs/linux/src/linux-3.10

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
	@rm -v *~
