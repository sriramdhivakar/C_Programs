SUBDIRS   = ex1-hello-world ex2-init-exit ex3-doc-license ex4-param ex5-multi-file ex6-char-register ex7-char-dev-register ex8-char-dev-register-dynamic ex9-write ex10-read ex11-debug ex12-proc ex13-ioctl
CLEANDIRS = ex1-hello-world ex2-init-exit ex3-doc-license ex4-param ex5-multi-file ex6-char-register ex7-char-dev-register ex8-char-dev-register-dynamic ex9-write ex10-read ex11-debug ex12-proc ex13-ioctl

all: 
	@for p in $(SUBDIRS); do \
		PWD=$(PWD)/$$p make -C $$p all; \
	done


clean:
	rm -f *~ *.ko
	for n in $(CLEANDIRS); do $(MAKE) -C $$n clean; done

