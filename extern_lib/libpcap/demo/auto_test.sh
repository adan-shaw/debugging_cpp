#!/bin/sh

sudo echo "*** test start ***"
sudo ./icmp_r_pcap &

sleep 1

sudo ./icmp_s_pcap &
echo "***  test end  ***"

jobs



# 需要root 权限执行AF_PACKET socket

