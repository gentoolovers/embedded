ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /home/david/Exynos4412/kernel/courseware/linux-3.14-fs4412/
PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module* module* test

.PHONY: modules modules_install clean

else
    obj-m := platform-driver.o
endif

