KVERS = $(shell uname -r)
 
# Kernel modules
obj-m += hello.o
 
# Specify flags for the module compilation.
#EXTRA_CFLAGS=-g -O0
 
build: kernel_modules user_test
 
kernel_modules:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules
user_test:
	gcc -o hello_test hello_test.c
 
clean:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) clean