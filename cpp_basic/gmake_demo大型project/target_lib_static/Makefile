#
# gnu makefile by auther adan_shaw@qq.com
# date: 2021/03/19
# << 预编译,编译,汇编,链接,全部分开来做的版本.可编译n个target >>
# << 一个源码目录,最多只有一个target生成 >>



.PHONY: default help check clean distclean all install uninstall pack
__PHONY_STRING=default help check clean distclean all install uninstall pack


# 编译工具前缀
#CROSS_PREFIX=arm-linux-androideabi
CROSS_PREFIX=

# 编译工具起别名
AS 	= $(CROSS_PREFIX)as
LD	= $(CROSS_PREFIX)ld
AR	= $(CROSS_PREFIX)ar
NM	= $(CROSS_PREFIX)nm
CC 	= $(CROSS_PREFIX)gcc
CXX = $(CROSS_PREFIX)g++
# 选择gcc / g++ 进行编译?? g++,gcc 有区别, 对c++ 的支持程度不一样
GCC = $(CXX)
# c/c++ 编译选项是不一样的, LDFLAGS 链接选项倒是一样的.
XXXFLAGS = CPPFLAGS
#XXXFLAGS = CFLAGS



#
# global variable(全局变量):
#
# project name 项目名
PROJECT_NAME=adan_srv

# source home源码目录(一个源码目录,最多只有一个target生成)
SOURCE_HOME=./src

# software install-path[软件安装路径]
PREFIX_PATH=/home/tarball/$(PROJECT_NAME)

# software compile-file home[软件编译文件存放路径]
BUILD_PATH=./build_$(PROJECT_NAME)

# version number[软件版本号] rc=测试版, R=release发行版
VERSION=1.1.1-20210319-rc
#VERSION=1.1.1-20210319-R

# host平台类型
OS=linux64

# compile output 编译输出:
#OUTPUT=$(PROJECT_NAME)-$(OS)-$(VERSION)
#OUTPUT=lib$(PROJECT_NAME)-$(OS)-$(VERSION).so
OUTPUT=lib$(PROJECT_NAME)-$(OS)-$(VERSION).a



#
# 编译gcc优化选项
#
# 编译32bit/64bit 的代码
CFLAGS += -m64
CPPFLAGS += -m64
#CFLAGS += -m32
#CPPFLAGS += -m32

# 编译成.a / .so 链接库时,必须添加-fPIC编译选项
CFLAGS += -fPIC
CPPFLAGS += -fPIC


# GNU / POSIX standard: { -D_GNU_SOURCE / -D_POSIX_SOURCE }
CFLAGS += -D_POSIX_SOURCE
CPPFLAGS += -D_POSIX_SOURCE

# no assert()禁用断言-DNDEBUG: { -DNDEBUG / empty }
CFLAGS += -DNDEBUG
CPPFLAGS += -DNDEBUG

# c version: { [ANSI C]=-std=c90=-ansi / -std=c99 / -std=c11 }
CFLAGS += -std=c99
# c version: { [ANSI C++]=-std=c++98 / -std=c++03 / -std=c++0x }
CPPFLAGS += -std=c++03

# open/close gcc-Warning(打开/关闭 gcc警告功能): { -w关闭 / -W开启 / empty=-W }
CFLAGS += -W
CPPFLAGS += -W

# compile optimization level编译优化等级: { -O0/-O1/-O2/-O3/-Os }
CFLAGS += -O2
CPPFLAGS += -O2

# debug level编译优化等级: { -g / -g3 / empty / ggdb / ggdb3 }
CFLAGS += -g3
CPPFLAGS += -g3

# GNU Performance debugging option性能调试选项: { -pg / empty }
#CFLAGS += -pg
#CPPFLAGS += -pg

# 禁用的编译选项, 麻烦, 多事, 作用不大, 所以不改, 默认值即可
#  -static 此选项将禁止使用动态库
#  -share 此选项将尽量使用动态库
#  -x 指定编译语言标准


# (其他补充的'CFLAGS/CPPFLAGS'编译选项)
CFLAGS += 
CPPFLAGS += 





# 编译-可执行程序, 不需要多加链接选项.
LDFLAGS += -lpthread





# 链接库/源代码的.h 头文件-路径指认
HEADFILE_PATH += -I./src_home/main_src -I./src_home/src1


# 静态链接库-(.a头文件)的-路径指认
LIB_STATIC_HEADFILE_PATH += -I./src_home/lib_static/lib_static1_headfile
LIB_STATIC_HEADFILE_PATH += -I./src_home/lib_static/lib_static2_headfile
# 静态链接库-(.a库文件)的-路径指认
LIB_STATIC_PATH += -L./src_home/lib_static/lib_static1
LIB_STATIC_PATH += -L./src_home/lib_static/lib_static2
# 静态链接库-LDFLAGS(ld链接器)的链接选项; 命名标准: [ lib_static.a = -l_static ]
LDFLAGS += -l_static -l_static2


# 动态链接库-(.so头文件)的-路径指认
LIB_DYNAMIC_HEADFILE_PATH += -I./src_home/lib_dynamic/lib_dynamic_headfile
# 动态链接库-(.so库文件)的-路径指认
LIB_DYNAMIC_PATH += -L./src_home/lib_dynamic/lib_dynamic
# 动态链接库-LDFLAGS(ld链接器)的链接选项; 命名标准: [ lib_dynamic.so = -l_dynamic ]
LDFLAGS += -l_dynamic

# '.so 动态链接库'后续:                                  (仅动态链接编译需要这么做)
#   编译好.exe程序之后,如果tty控制台需要运行程序,
#   则仍然需要export导入LD_LIBRARY_PATH tty环境变量,让tty找到.exe相关的.so动态链接库
#export LD_LIBRARY_PATH=LD_LIBRARY_PATH:../src_home/lib_dynamic/lib_dynamic
# ps: 
#   export注册的时候,可以使用'相对路径',前提是你的.exe存放路径,相对.so链接库路径正确.

# tty 的环境变量, 默认LD_LIBRARY_PATH为空.

# 如果不想使用export导入LD_LIBRARY_PATH动态链接库变量,
# 你需要将动态库移动到:/lib or /usr/lib 中.

# 第三方: 链接库和头文件-汇总
MY_HEADFILE_POOL := \
$(LIB_STATIC_HEADFILE_PATH) $(LIB_DYNAMIC_HEADFILE_PATH) $(HEADFILE_PATH) 
MY_LIB_POOL := \
$(LIB_STATIC_PATH) $(LIB_DYNAMIC_PATH)





# 指认所有.c / .cpp 源文件路径
__SRC_POOL := ./src_home/main_src/*.cpp ./src_home/src1/*.cpp
#__SRC_POOL := ./src_home/main_src/*.c ./src_home/src1/*.c

# 指认编译过程中, 所有.i .s .o 等中间文件, 生成时的存放路径
__I_POOL := $(BUILD_PATH)/*.i
__S_POOL := $(BUILD_PATH)/*.s
__O_POOL := $(BUILD_PATH)/*.o 

# 解析1: 
# 	*.cpp/*.c通配符, 实际是安全的,
# 	只是生成的'.o/.s 文件'会全部集中在当前目录下面.
# 解析2: 
# 	main() 所在的文件, 应该放到最后!!(编译顺序问题)
# 	有时候会找不到main() 函数, 调整一下<文件路径>的顺序即可.
# 	一般使用'*'通配符, 顺序问题都是自适应解决的.
# 解析3: 
# 	你不需要指出.h文件的具体文件名, 
# 	gcc 会自己根据xxx.cpp的命名, 自动在已知的.h头文件目录中, 寻找xxx.h相关联的头文件.
# 	如果你需要-MM 检查一下, gcc 也会自己查找xxx.cpp 同名隐晦的相关依赖文件,
# 	出现任何问题, 同样会报告错误.








# 如果build home文件夹不存在, 则自动创建
$(shell if [ ! -d $(BUILD_PATH) ]; then mkdir $(BUILD_PATH); fi;)



# makefile 默认目标
default:all



help:
	@echo -e "[target list]:\n\t{" $(__PHONY_STRING) "}"
	@echo -e "demo:\n\tmake all\n"
	@echo -e "makefile mission-help finished, makefile about to quit.\n\n"


clean:
	@echo "clean = delete $(BUILD_PATH)/$(OUTPUT) only"
	if [ -f $(BUILD_PATH)/$(OUTPUT) ]; then rm $(BUILD_PATH)/$(OUTPUT); fi;
	@echo -e "makefile mission-clean finished, makefile about to quit.\n\n"


distclean:
	@echo "distclean = delete build home: $(BUILD_PATH)"
	if [ -d $(BUILD_PATH) ]; then rm -r $(BUILD_PATH); fi;
	@echo -e "makefile mission-distclean finished, makefile about to quit.\n\n"


check:
	@echo ">> all source file(所有源文件) <<"
	@echo $(__SRC_POOL)
	@echo -e "\n\n"
	@echo ">> checking all src(-MM编译前检查) <<"
	$(GCC) -MM $(__SRC_POOL)
	@echo -e "\n\n"
	@echo ">> make -E src(-E预编译处理) <<"
	$(GCC) -E $(__SRC_POOL)
	@echo -e "\n\n"
	@echo -e "build: $$(uname -s)-$(shell uname -r)\n"
	@echo -e "host: $(OS)\n"
	@echo -e "targer: $$(gcc -v)\n\n\n"
	@echo "Output Name: $(OUTPUT)"
	@echo "Prefix Path: $(PREFIX_PATH)"
	@echo "Build Path: $(BUILD_PATH)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "CPPFLAGS: $(CPPFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo -e "makefile mission-check finished, makefile about to quit.\n\n"



all:
	@echo ">> building all src file <<"
	# 预编译
	if [ ! -d $(BUILD_PATH) ]; then mkdir $(BUILD_PATH); fi;
	for tmp in $(__SRC_POOL); \
	do \
		# basename 取文件名 去除.cpp后缀 \
		echo "execute precompile" "$$tmp" "->" "$$(basename $$tmp .cpp).i"; \
		$(GCC) $($(XXXFLAGS)) $(MY_HEADFILE_POOL) -E "$$tmp" -o "$(BUILD_PATH)/$$(basename $$tmp .cpp)".i; \
	done;
	# 编译
	for tmp in $(__I_POOL); \
	do \
		echo "execute compile" "$$tmp" "->" "$$(basename $$tmp .i).s"; \
		$(GCC) $($(XXXFLAGS)) $(MY_HEADFILE_POOL) -S "$$tmp" -o "$(BUILD_PATH)/$$(basename $$tmp .i)".s; \
	done;
	# 汇编
	for tmp in $(__S_POOL); \
	do \
		echo "execute assembly" "$$tmp" "->" "$$(basename $$tmp .s).o"; \
		$(GCC) $($(XXXFLAGS)) -c "$$tmp" -o "$(BUILD_PATH)/$$(basename $$tmp .s)".o; \
	done;
	# 链接生成.exe
	#$(GCC) $(__O_POOL) $(MY_LIB_POOL) $(LDFLAGS) -o $(BUILD_PATH)/$(OUTPUT);
	# 链接生成.so 动态链接库
	#$(GCC) $(__O_POOL) -shared $(LDFLAGS) $(MY_LIB_POOL) -o $(BUILD_PATH)/$(OUTPUT);
	# 链接生成.a 静态链接库
	ar -crv $(BUILD_PATH)/$(OUTPUT) $(__O_POOL);
	# 调试:
	#   这样的顺序是错误的, 链接的顺序很重要!!
	#$(GCC) -v $(MY_LIB_POOL) $(LDFLAGS) -o $(OUTPUT) $(__O_POOL)
	@echo -e "makefile mission-all finished, makefile about to quit.\n\n"



# 老方法(预编译和编译没有拆开,make clean没办法清除缓存数据,需要使用:find . -name "*.s")
all2:
	@echo ">> building all src file <<"
	# '预编译&编译'一起做.
	$(GCC) $($(XXXFLAGS)) $(MY_HEADFILE_POOL) -S $(__SRC_POOL)
	# 汇编
	$(GCC) $($(XXXFLAGS)) -c ./*.s
	# 链接生成.exe
	#$(GCC) ./*.o $(MY_LIB_POOL) $(LDFLAGS) -o $(BUILD_PATH)/$(OUTPUT);
	# 链接生成.so 动态链接库
	#$(GCC) ./*.o -shared $(LDFLAGS) $(MY_LIB_POOL) -o $(BUILD_PATH)/$(OUTPUT);
	# 链接生成.a 静态链接库
	ar -crv $(BUILD_PATH)/$(OUTPUT) ./*.o;
	@echo -e "makefile mission-all2 finished, makefile about to quit.\n\n"



install:
	@echo ">> mission-install needs root authority <<"
	@echo ">> we'll create to the prefix path: $(PREFIX_PATH) <<"
	$(shell if [ ! -d $(PREFIX_PATH) ]; then mkdir $(PREFIX_PATH); fi;)
	# 拷贝&&覆盖
	cp $(OUTPUT) $(PREFIX_PATH)/$(OUTPUT)
	cp $(MY_LIB_POOL) $(PREFIX_PATH)/$(MY_LIB_POOL)
	@echo "makefile mission-install finished, makefile about to quit."


uninstall:
	@echo ">> mission-uninstall needs root authority <<"
	@echo ">> we'll delete to the prefix path: $(PREFIX_PATH) <<"
	if [ -d $(PREFIX_PATH) ]; then rm -r $(PREFIX_PATH); fi;
	@echo "makefile mission-uninstall finished, makefile about to quit."



pack: distclean
	# pack up source codes 打包源代码
	@echo ">> execute mission-distclean first <<"
	@echo ">> and then pack up everything to be a ./$(PROJECT_NAME)-$(VERSION).tar.bz2 <<"
	$(shell if [ -f "./$(PROJECT_NAME)-$(VERSION).tar.bz2" ]; then rm ./$(PROJECT_NAME)-$(VERSION).tar.bz2; fi;)
	tar jcf ./$(PROJECT_NAME)-$(VERSION).tar.bz2 ./*
	@echo "makefile mission-pack finished, makefile about to quit."





# jump 语句, 防止出错.
# makefile 任务标记, 是可以重复定义的, 
# 这里的jump 任务标记, 转跳到'error错误段'(仅作转跳示范)
jump:error
jump:error
jump:error



#
# makefile inside-target mission(makefile内部target mission,不对外开放)
#
error:
	@echo "**********error**********"
	@echo "*makefile error and quit*"
	@echo "**********error**********"
	@echo "makefile mission-error finished, makefile about to quit."


