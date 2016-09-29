export ARCH=arm
export CROSS_COMPILE=/home/arm-2014.05/bin/arm-none-linux-gnueabi-

ifneq ($(KERNELRELEASE),)
	obj-m := fskey.o
else
	KERNELDIR := /home/kernel/linux-4.5.5
	PWD       := $(shell pwd)

modules:
	make -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm -rf *.o *.symvers *.order *.ko *.mod.c *.markers .tmp_versions .*.cmd
