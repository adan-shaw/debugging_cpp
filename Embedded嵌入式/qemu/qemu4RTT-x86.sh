#!/bin/sh



# 创建一个./sd.bin, 作为磁盘(64MB)
if [ ! -f "sd.bin" ]; then
dd if=/dev/zero of=sd.bin bs=1024 count=65536
mkfs.fat sd.bin
fi

# 启动./rtthread.bin
qemu-system-x86_64 -cpu 486 -m 128M -smp 4 -cdrom ./rtthread.bin -nographic \
-drive if=none,file=./sd.bin,format=raw,id=blk0 \
-chardev socket,host=127.0.0.1,port=4321,server=on,wait=off,telnet=on,id=console0





# 检查./rtthread.bin 是否启动, 自己查看串口127.0.0.1:4321 (看到LISTEN 就是RTT 起来了)
netstat -tuln | grep 4321

# 如何杀死qemu 运行中的RTT(只能用kill 了)
htop

# 看到100% 占用率的就是RTT, 实时程序都会卡死的, 直接杀死:
#kill -s 9 <pid>

