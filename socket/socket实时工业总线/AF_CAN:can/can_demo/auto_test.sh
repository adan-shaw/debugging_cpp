#!/bin/sh

# 编译:
gcc -g3 ./can_recv.c -o r_can
gcc -g3 ./can_send.c -o s_can



# 测试环境布置(新增一个ip can虚拟网络空间, 并命名为: vcan0):
sudo ip link add dev vcan0 type vcan
# 假设设置为125kbit/s的位速率(这一步可以省略, 如果只是为了仿真模拟CAN 网络的话)
sudo ip link set vcan0 type can bitrate 125000 
sudo ip link set up vcan0



# 执行测试:
./r_can &
./s_can
