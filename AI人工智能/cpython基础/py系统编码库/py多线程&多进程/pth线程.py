#coding=UTF-8



# python 多线程
'''
	python 同样支持多线程, python 多线程会共享全局变量, 共享同一个gc, 这样会导致以下缺点:
		* 线程同步压力更大
		* gc 缓存更大(引发多线程互相占用嵌套, gc 必然会有大量内存收不回来)
		* python 多线程调试能力太差, 崩溃难调试
		* python 多线程库接口没有c++ 齐全, 要做高性能精确运算, 就别用脚本折腾了, 浪费时间;

	所以, python 多线程更加不能滥用, 
	想要尝试一下多线程蛋糕, script 脚本建议直接用Coroutine协程乐乐就算了, python 原则上还是一个单进程脚本机;
	单线程脚本, 不用考虑线程同步, 单进程裸奔即最好!! 简单, 敏捷开发;



	python 线程同步有: 锁, 条件变量, 信号量, 信号, 共享内存, 等等;(c++ 可以做的, python 都可以做)
'''



import threading
import time



def pth_func1():
	for i in range(3):
		time.sleep(1)
		print("Task 1: {i}")

def pth_func2():
	for i in range(3):
		time.sleep(1.5)
		print("Task 2: {i}")

# 创建多个线程
pth1 = threading.Thread(target=pth_func1)
pth2 = threading.Thread(target=pth_func2)

# 启动线程
pth1.start()
pth2.start()

# 等待两个线程完成(阻塞)
pth1.join()
pth2.join()

print("Both threads have finished")

