$(warning KERNELRELEASE=$(KERNELRELEASE))
ifeq ($(KERNELRELEASE),)

#KERNELDIR ?= /home/linux/FS4412/linux-3.0-fs4412_v8/

KERNELDIR ?= /lib/modules/$(shell uname -r)/build  
PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) 
	
modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module* modules*

.PHONY: modules modules_install clean

else
    obj-m := hello.o
endif

