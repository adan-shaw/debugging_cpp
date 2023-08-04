# makefile:
# 	./target_exe: 生成可执行程序
# 	./target_lib_dynamic: 生成动态库
# 	./target_lib_static: 生成静态库
# ps:
# 	gcc 编译器, 路径不能有任何中文和特殊字符, 否则报错


target_exe=./target_exe
target_lib_dynamic=./target_lib_dynamic
target_lib_static=./target_lib_static
MAKE=make

# project name 项目名
PROJECT_NAME=adan_srv

# version number[软件版本号] rc=测试版, R=release发行版
VERSION=1.1.1-20210319-rc
#VERSION=1.1.1-20210319-R


# makefile 默认目标
default:all



all:
	cd $(target_exe) && $(MAKE) all
	cd $(target_lib_dynamic) && $(MAKE) all
	cd $(target_lib_static) && $(MAKE) all


clean:
	cd $(target_exe) && $(MAKE) clean
	cd $(target_lib_dynamic) && $(MAKE) clean
	cd $(target_lib_static) && $(MAKE) clean


distclean:
	cd $(target_exe) && $(MAKE) distclean
	cd $(target_lib_dynamic) && $(MAKE) distclean
	cd $(target_lib_static) && $(MAKE) distclean


pack: distclean
	# pack up source codes 打包源代码
	@echo ">> execute mission-distclean first <<"
	@echo ">> and then pack up everything to be a ./$(PROJECT_NAME)-$(VERSION).tar.bz2 <<"
	$(shell if [ -f "./$(PROJECT_NAME)-$(VERSION).tar.bz2" ]; then rm ./$(PROJECT_NAME)-$(VERSION).tar.bz2; fi;)
	tar jcf ./$(PROJECT_NAME)-$(VERSION).tar.bz2 ./*
	@echo "makefile mission-pack finished, makefile about to quit."
