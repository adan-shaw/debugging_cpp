# -*- coding: utf-8 -*-

'''
	关于python 线程池, 现在用的比较多的是future 未来模块;
	from concurrent.futures import ThreadPoolExecutor;
	(老式的threadpool 已经比较少人用了)

	# 安装python-futurist
	apt-get install python-futurist python3-futurist



	python-futurist - useful additions to futures, from the future - Python 2.x
	python3-futurist - useful additions to futures, from the future - Python 3.x
'''



from concurrent.futures import ThreadPoolExecutor
from concurrent.futures import wait, ALL_COMPLETED, FIRST_COMPLETED, as_completed
import time



def pth_func(a):
	print("hello: "+a)
	time.sleep(1)



def main():
	dict_tmp=["a","b","c"]
	start=time.time()
	for tmp in dict_tmp:
		pth_func(tmp)
	end=time.time()
	print("*1-time: "+str(end-start))

	start=time.time()
	with ThreadPoolExecutor(3) as pth_pool:
		for tmp in dict_tmp:
			# pth_pool.submit()
			pth_pool.submit(pth_func,tmp)
	end=time.time()
	print("*3-time: "+str(end-start))

	start=time.time()
	with ThreadPoolExecutor(3) as pth_pool:
		# pth_pool.map()
		pth_pool.map(pth_func,dict_tmp)
	end=time.time()
	print("*4-time: "+str(end-start))



if(__name__ == '__main__'):
	main()



#
# 使用list 来管理线程池
#
dict_tmp=["a","b","c"]
start=time.time()
pth_pool_list = []
pth_pool = ThreadPoolExecutor(max_workers=3)
for tmp in dict_tmp:
	# 将每个线程放进list[] 中进行管理
	pth_pool_list.append(pth_pool.submit(pth_func,tmp))

# main 线程会跑得比pth_pool 线程池快, 这里需要使用wait()
tmp=3
while(tmp>0):
	tmp = tmp - 1
	# wait() 任意线程结束返回(不推荐)
	wait(pth_pool_list, return_when=FIRST_COMPLETED)

# wait() 所有线程结束返回(推荐)
wait(pth_pool_list, return_when=ALL_COMPLETED)

end=time.time()
print("*2-time: "+str(end-start))

# 获取所有的线程返回值
pth_ret = as_completed(pth_pool_list)

# 打印所有的线程返回值
for tmp in pth_ret:
	print("* ", tmp.result())
