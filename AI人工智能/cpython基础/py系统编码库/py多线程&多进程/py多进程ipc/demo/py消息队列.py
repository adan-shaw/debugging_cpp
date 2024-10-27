#coding=UTF-8

import multiprocessing



def worker(q):
	while(True):
		# 子进程从'消息队列'获取数据(阻塞获取)
		item = q.get()
		# 如果*.get() = None, 则表示获取'消息队列'的msg完毕
		if item is None:
			break
		print(item)



if(__name__ == '__main__'):
	# 创建'消息队列'
	q = multiprocessing.Queue()

	# 创建4 个子线程
	# (传递参数时, 必须使用: args=(shared_val,) 多加一个','号在'()'里面, 否则报错: TypeError: 'Synchronized' object is not iterable)
	proc_pool_son = [multiprocessing.Process(target=worker, args=(q,)) for tmp in range(4)]

	# 启动这4 个子线程
	for son in proc_pool_son:
		son.start()

	# 向'消息队列'写入非空数据
	for i in range(10):
		q.put(i)

	# 向'消息队列'写入空数据(一般情况下, 不能写入None 数据!! 没有什么编程上的意义)
	for tmp in range(len(proc_pool_son)):
		q.put(None)

	# 等待这4 个子线程结束
	for son in proc_pool_son:
		son.join()
