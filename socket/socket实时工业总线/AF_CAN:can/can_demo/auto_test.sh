#!/bin/sh

# 编译:
gcc -g3 ./can_recv.c -o r_can
gcc -g3 ./can_send.c -o s_can



# 测试环境布置(新增一个ip can虚拟网络空间, 并命名为: vcan0):
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0



# 执行测试:
./r_can &
./s_can
