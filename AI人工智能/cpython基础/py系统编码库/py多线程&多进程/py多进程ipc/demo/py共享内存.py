#coding=UTF-8

import multiprocessing



'''
	python 的共享内存, 一般存放两种数据:
		* 常见的Value 变量(常规变量, 不能是容器, 一般是数值)
		* array 数组(一般是数组/字符串)

	共享内存可以让多个进程之间共享同一块内存区域, 这样就可以避免进程之间频繁地复制数据;
	在主进程中使用Value或Array创建一个共享内存对象, 
	然后将其作为参数传递给子进程, 在子进程中可以直接修改共享内存对象中的值, 主进程也可以直接读取共享内存对象中的值;

	ps:
		共享内存一般做只读共享访问, 如果做写共享访问, 需要加信号灯进行线程同步, 跟c++ 一样;
'''


def proc_son_func(shared_val):
	shared_val.value += 1



# 只演示value 的使用, array 的使用demo, 略!!
if(__name__ == '__main__'):
	# 创建一个共享内存变量('i' 表示int, 不是共享内存的变量名, 而是共享内存的变量type)
	shared_val = multiprocessing.Value('i', 0)

	# 创建10 个子进程, 统一执行proc_son_func(), 把共享内存变量统一传给子进程
	# (传递参数时, 必须使用: args=(shared_val,) 多加一个','号在'()'里面, 否则报错: TypeError: 'Synchronized' object is not iterable)
	proc_son_pool = [multiprocessing.Process(target=proc_son_func, args=(shared_val,)) for tmp in range(10)]

	# 启动每个子进程
	for son in proc_son_pool:
		son.start()

	# 等待每个子进程结束
	for son in proc_son_pool:
		son.join()

	# 输出10 (每个子进程都对i+1)
	print(shared_val.value)



# ps: 
# 	py2/py3 的数目差了1, py2=9, py3=10, 应该是range(10) 的差异问题导致的, 并不是程序逻辑的问题
