KVERS = $(shell uname -r)
SRC = $(wildcard*.o)

KERNEL_HEARDERS_HATH = /usr/src/linux-headers-$(KVERS)

obj-m += char_test.o

build: kernel_modules

kernel_modules:
	make -C $(KERNEL_HEARDERS_HATH) M=$(CURDIR) modules
clean:
	make -C $(KERNEL_HEARDERS_HATH) M=$(CURDIR) clean
