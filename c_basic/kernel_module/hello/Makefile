#
# 本机pc 预览版
#

# 指定kernel 模块的源文件=./hello.c, 中间文件=./hello.o, 导出文件=./hello.ko
obj-m:=hello.o



#
# 编译krenel 子模块前要做的准备:
#
# * 安装依赖(选择与本机一致的kernel):
# 	apt-get install linux-headers-4.19.0-18-all
# 	apt-get install linux-source
# 	apt-get install libncurses5-dev
#
# * makefile 命名规则: Makefile, 不能是makefile;
#
# * 所有路径, 都不能有特殊字符, 最好使用全英文字符路径;



# 使用debian 的kernel 版本( apt-get install linux-headers-4.19.0-18-all )
KERN_DIR ?= /usr/src/linux-headers-$(shell uname -r)



# 指定cross 编译工具链(略, 本机预览版, 随便使用默认gcc 即可编译)



default:
	make -C $(KERN_DIR) M=$(shell pwd) modules

clean:
	make -C $(KERN_DIR) M=$(shell pwd) clean

