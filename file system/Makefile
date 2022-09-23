TARGET_MODULE:=myfs
ifeq ($(KERNELRELEASE),)
    KERNELDIR ?= /lib/modules/$(shell uname -r)/build
    PWD := $(shell pwd)
 
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
 
modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
 
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.order *.symvers

insmod:
	insmod ./${TARGET_MODULE}.ko

rmmod:
	rmmod ./${TARGET_MODULE}.ko

mknod:
	mknod /dev/${TARGET_MODULE} c 251 0
	chmod 777 /dev/${TARGET_MODULE}

.PHONY: modules modules_install clean
 
else
    # called from kernel build system: just declare what our modules are
    obj-m := ${TARGET_MODULE}.o
 
endif
