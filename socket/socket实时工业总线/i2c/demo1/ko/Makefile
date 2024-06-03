obj-m:=gpio.o



# 编译krenel 子模块前的准备
# * 最好使用与PC 机相同的linux 内核project源码做测试, 这样成功率高, 不需要预先编译kernel 项目;
#   sudo apt-get install libncurses5-dev
#   apt-get install linux-source
#
# * 必须执行是make config/make menuconfig first, 然后编译kernel 内核, 完成编译后, 才能编译krenel 子模块;
#   (或者你使用与PC 机相同的kernel 版本, 就不需要编译内核了, 否则都要先编译内核, 再编译kernel 子模块!!)
#
# * makefile 必须命名为: Makefile, 一个字都不能改;
#
# * 所有路径, 都不能有特殊字符, 最好使用全英文字符路径;



# 编译kernel 子模块, 需要指明linux kernel project源码路径
#KERN_DIR:=./linux-2.6.39.4

# 自动查看kernel 版本号(使用与PC 机相同的linux 内核project源码做测试, 这样成功率高)
KERN_DIR ?= /usr/src/linux-headers-$(shell uname -r)



default:
	make -C $(KERN_DIR) M=$(shell pwd) modules

clean:
	make -C $(KERN_DIR) M=$(shell pwd) clean

