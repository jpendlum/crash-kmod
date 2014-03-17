obj-m := crash-kmod.o

KERNEL_SRC := /lib/modules/$(shell uname -r)/build

SRC := $(shell pwd)

.PHONY : install

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC)

install: modules_install
	cp crash-kmod.h /usr/include/

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install

uninstall:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) clean
	rm /usr/include/crash-kmod.h

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers
