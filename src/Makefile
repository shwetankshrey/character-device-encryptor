ifneq ($(KERNELRELEASE),)
	obj-m += encrypto.o
	obj-m += decrypto.o
else
	KERNELDIR ?= /lib/modules/4.4.0-119-generic//build
	PWD  := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
endif