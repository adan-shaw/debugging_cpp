#!/bin/sh

cd ./ko
make



# 代码测评:
# 	看不懂原作者的'位操作原意', 也没有实物电路图, 不知道具体'位操作原意图', 没办法对'位操作'代码进行纠正;
# 	但原作者代码中的bug, 基本已经修复, 丝滑debugging ok!!



# 需要root 权限



mod_name="gpio"
KERN_ALERT_KEYWORD="gpio"


# 1.查看当前已经加载的驱动程序:
sudo lsmod | grep $mod_name


# 2.加载驱动:
# 强制卸载(卸载只需要驱动名即可)
sudo rmmod $mod_name
# 重新装载ko 模块(如果linux 的.ko 驱动陷入死循环, 一直被占用, linux 可能连关机都很麻烦, 卡很久, 因此: linux 内核调试, 建议开虚拟机, 免得搞坏了系统)
sudo insmod ./$mod_name.ko


# 3.查看内核打印信息(不用重启, 直接查看kernel 模块装载时, KERN_ALERT() syscall 的输出, 相当于prinft()): 只能追逐你要打印的字符串, 不能根据kernel 模块名进行详细追逐;
sudo dmesg | grep $KERN_ALERT_KEYWORD

# 查看是否产生了gpio 设备name /dev/my_gpio
ls /dev | grep my_gpio


# 4.卸载驱动:
#sudo rmmod $mod_name


# 5.查看模块的详细信息:
sudo modinfo ./$mod_name.ko





cd ..

gcc -g3 ./app.c -o x

# 执行app.c, 这个程序会往/dev/my_gpio 中写数据, gpio.ko 收到i2c 数据, 会执行对应的操作
./x
