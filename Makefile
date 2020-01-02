KERNDIR:=/lib/modules/$(shell uname --release)/build
PWD:=$(shell pwd)
mymodule-objs := charModule.o
obj-m:= charModule.o

default:
	$(MAKE) -C $(KERNDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNDIR) M=$(PWD) clean
