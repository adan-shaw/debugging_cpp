#coding=utf-8



# 1.Coroutine协程的安装使用(pypy 没有gevent 模块的实现)
'''
	协程并不是python 的标准库, 需要自己手动安装:
		greenlet模块: 无同步的协程basic功能实现
		Gevent模块:   有同步的协程功能实现
	apt-get install python3-greenlet python3-gevent
	pip3 install greenlet gevent

	apt-get install python-greenlet python-gevent

	# websocket 协程模块??
	pip3 install geventwebsocket
'''



# 2.协程简介
'''
协程的自动挂起切换:
	linux 线程在遇到阻塞操作时, 会自动挂起, 让出cpu 使用权;
	python 协程同样也会在遇到阻塞操作时, 自动挂起, 让出cpu 使用权, 只是调度级别不一样而已;

	linux 是系统级调度, 速度较慢, 因为系统还有其他大量进程在等待调度, 所以性能较慢;
	python 协程调度, 速度较快, 因为等待调度的协程较小, 所以性能高;


协程优点: 
	1.协程的切换调度, 开销更小, 性能更好;
	2.二次规划单线程内的多任务制度, 并且可以减少使用锁, 十分美妙;

协程缺点:
	1.协程的本质是单线程下, 无法利用多核cpu;
	2.协程一旦协程出现永久阻塞, 死等, 崩溃, 将会整个线程阻塞, 死等, 崩溃;
'''



# 3.协程的创建API
'''
python 协程的创建使用十分简单, 只有3 个操作:
	创建协程块:
		coro=greenlet(func_api)
	切换/启动:
		coro.switch()
	删除协程块:
		del coro
'''
import greenlet
import time

# 打印greenlet 模块的所有变量
#print(greenlet)

def func_daddy():
	while 1:
		print("call me daddy")
		time.sleep(1)

coro=greenlet.greenlet(func_daddy)
#coro.switch()
#del coro



# 4.协程之间的同步
'''
python 协程的创建使用十分简单, 只有3 个操作:
	创建&启动协程块:
		coro=gevent.spawn(func_api)
	等待单个协程块结束:
		coro.join()
	等待多个协程块结束:
		gevent.joinall([coro1,coro2,coro3])
	协程块休眠(触发协程切换):
		gevent.sleep(1)
	删除协程块:
		del coro
'''

import gevent

def func_mommy(x,y,z):
	#while 1:
	if 1:
		print("F mommy")
		gevent.sleep(1)
	return "then happy"

# 创建&启动协程块时, 顺带传入参数
coro=gevent.spawn(func_mommy,x=1,y=2,z=3)

coro.join()

ret=coro.value
