#!/bin/sh

# 本机预览版, 需要root 权限
# 交叉编译版, 需要部属到target 交叉编译目标板子上, 才能执行!! (否则推荐运行hello PC 本机版, 严重一下业务逻辑即可, 不必做交叉编译)



mod_name="hello"
KERN_ALERT_KEYWORD="Hello"


# 1.查看当前已经加载的驱动程序:
sudo lsmod | grep $mod_name


# 2.加载驱动:
sudo insmod ./$mod_name.ko


# 3.查看内核打印信息[不用重启, 直接查看kernel 模块装载时, KERN_ALERT() syscall 的输出, 相当于prinft()]
# 	只能追逐你要打印的字符串, 不能根据kernel 模块名进行详细追逐;
sudo dmesg | grep $KERN_ALERT_KEYWORD


# 4.卸载驱动:
sudo rmmod $mod_name


# 5.查看模块的详细信息:
sudo modinfo ./$mod_name.ko
