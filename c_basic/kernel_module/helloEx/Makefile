#
# cross 交叉编译版
#

# 指定kernel 模块的源文件=./hello.c, 中间文件=./hello.o, 导出文件=./hello.ko
obj-m:=hello.o



# 指定kernel 头文件的源码路径(源码解压路径即可, 不需要进入解压后的任何二级目录)
KERN_DIR ?= /home/adan/fcgi/buildroot-2023.02.11-qemu_aarch64_virt_defconfig/output/build/linux-5.15.185/

# 使用debian 的kernel 版本( apt-get install linux-headers-4.19.0-18-all )
#KERN_DIR ?= /usr/src/linux-headers-$(shell uname -r)



# 指定cross 编译工具链(内核原则上只需要$(CC) 即可)
myCC = /home/adan/fcgi/buildroot-2023.02.11-qemu_aarch64_virt_defconfig/output/host/bin/aarch64-buildroot-linux-musl-gcc.br_real
myCXX = $(myCC)
myLD = /home/adan/fcgi/buildroot-2023.02.11-qemu_aarch64_virt_defconfig/output/host/aarch64-buildroot-linux-musl/bin/ld

default:
	# 必须要指定ARCH=arm64 (选择前, 可以查看$(KERN_DIR) 支持那些架构, 你的编译工具链是什么架构, 你选择了哪个架构的芯片)
	ls $(KERN_DIR)/arch
	# M=源码目录路径, 编译参数清一色传递modules, 不能更改 [固定参数组合]
	# (kernel 是一个大型宏内核project, 你编译的一切, 都叫内核子模块而已, 即modules)
	make ARCH=arm64 CC="$(myCC)" CXX="$(myCC)" LD="$(myLD)" -C $(KERN_DIR) M=$(shell pwd) modules

clean:
	# 清空资源时, 可以不指定那么多编译参数
	make ARCH=arm64 -C $(KERN_DIR) M=$(shell pwd) clean

