# Makefile for Debian 4.0 rc3

# 一次只能编译一个.ko, 因此需要拆分!! 最好拆分
outcome := hello_world

# 指定输出hello_world.ko (如果只有一个hello_world.o, 可以不用module-objs := , 如果有多个*.o, 则需要module-objs := )
obj-m := $(outcome).o
#module-objs := hello_world.o

KERNELDIR = /lib/modules/`uname -r`/build
PWD = `pwd`



default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

# 安装.ko 内核模块
install:
	insmod $(outcome).ko

# 卸载.ko 内核模块
uninstall:
	rmmod $(outcome).ko

# 清空编译中间产物
clean:
	rm -rf *.o *.mod.c *.ko
	rm -rf Module.symvers .*cmd .tmp_versions
	rm -rf *.order
