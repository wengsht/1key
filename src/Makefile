obj-m := ok_crypto.o
ok_crypto-objs := ok_main.o ok_file.o ok_mem.o
EXTRA_CFLAGS+=-I$(PWD)/include

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
MAKE := make

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

test:
	make 
	sudo ./ok_init exit
	sudo ./ok_init
	echo 1123123 > /dev/ok_crypto0
	cat /dev/ok_crypto0
clean:
	rm *.o *.ko *.symvers *.order *.mod.c
	
