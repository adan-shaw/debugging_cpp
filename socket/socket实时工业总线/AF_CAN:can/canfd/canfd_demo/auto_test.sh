#!/bin/sh

# 编译:
gcc -g3 ./canfd_recv.c -o r_canfd
gcc -g3 ./canfd_send.c -o s_canfd



# 测试环境布置(新增一个ip can虚拟网络空间, 并命名为: vcan0):
sudo ip link add dev vcanfd0 type vcan
sudo ip link set up vcanfd0



# 执行测试:
./r_canfd &
./s_canfd
