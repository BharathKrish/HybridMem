obj-m += task_test.o

all:
	make -C /lib/modules/3.2.0-101-generic/build M=$(PWD) modules
clean:
	make -C /lib/modules/3.2.0-101-generic/build M=$(PWD) clean
