
ifeq ($(KERNELRELEASE),)
	
PWD=$(shell pwd)
KERNELDIR=/home/linux/toolchain/linux-3.14
#KERNELDIR=/lib/modules/$(shell uname -r)/build

modules:
	make -C $(KERNELDIR)  M=$(PWD)

clean:
	make -C $(KERNELDIR) M=$(PWD) clean 


else 
	
	obj-m := fs4412_key.o 

endif 
