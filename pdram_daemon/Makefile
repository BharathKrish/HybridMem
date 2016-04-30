obj-m += wq_test.o
obj-m += prashanth_migrate.o

all:
	make -C ../linux-4.5 M=$(PWD) modules
clean:
	make -C ../linux-4.5 M=$(PWD) clean
