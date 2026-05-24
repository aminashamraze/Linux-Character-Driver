obj-m += simple_char_driver.o
KER := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
  $(MAKE) -C $(KER) M=$(PWD) modules
clean:
  $(MAKE) -C $(KER) M=$(PWD) clean
