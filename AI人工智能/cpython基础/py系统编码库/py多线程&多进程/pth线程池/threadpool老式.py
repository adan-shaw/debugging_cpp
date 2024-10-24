# -*- coding: utf-8 -*-

'''
	threadpool 是一个比较老的模块了, 现在虽然还有一些人在用, 但已经不再是主流了;
	关于python 线程池, 现在用的比较多的是future 未来模块;
	from concurrent.futures import ThreadPoolExecutor;

	# 安装pip
	sudo apt-get install python-pip

	# 安装threadpool (只能靠pip 来安装了, 模块太老了)
	sudo pip install threadpool
'''



import threadpool
import time



def pth_func (a):
	print("hello: "+a)
	time.sleep(1)



def main():
	global result
	dict_tmp=["a","b","c"]
	start=time.time()

	pth_pool=threadpool.ThreadPool(5)
	requests=threadpool.makeRequests(pth_func,dict_tmp)
	for req in requests:
		pth_pool.putRequest(req)

	pth_pool.wait()
	end=time.time()
	print("time: "+str(end-start))



if(__name__ == '__main__'):
	main()
