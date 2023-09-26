#coding=utf8



import time



# 返回cpu 时钟数字(浮点)
print(time.clock())



# 返回时间数字(浮点)
print(time.time())



# 返回字符串格式化时间, 如: Tue Dec 21 19:12:09 2021
print(time.ctime())



# 进程休眠(单位秒), 必然返回None
time.sleep(1)
# 错误写法(打印"None"无意义)
#print(time.sleep(1))




#
# time 模块API list
#
'''
time.altzone
	返回格林威治西部的夏令时地区的偏移秒数.如果该地区在格林威治东部会返回负值(如西欧, 包括英国).对夏令时启用地区才能使用.

time.asctime([tupletime])
	接受时间元组并返回一个可读的形式为"Tue Dec 11 18:07:14 2008"(2008年12月11日 周二18时07分14秒)的24个字符的字符串.

time.clock()
	用以浮点数计算的秒数返回当前的CPU时间.用来衡量不同程序的耗时, 比time.time()更有用.

time.ctime([secs])
	作用相当于asctime(localtime(secs)), 未给参数相当于asctime()

time.gmtime([secs])
	接收时间戳(1970纪元后经过的浮点秒数)并返回格林威治天文时间下的时间元组t.注:t.tm_isdst始终为0

time.localtime([secs])
	接收时间戳(1970纪元后经过的浮点秒数)并返回当地时间下的时间元组t(t.tm_isdst可取0或1, 取决于当地当时是不是夏令时).

time.mktime(tupletime)
	接受时间元组并返回时间戳(1970纪元后经过的浮点秒数).

time.sleep(secs)
	推迟调用线程的运行, secs指秒数.

time.strftime(fmt[,tupletime])
	接收以时间元组, 并返回以可读字符串表示的当地时间, 格式由fmt决定.

time.strptime(str,fmt='%a %b %d %H:%M:%S %Y')
	根据fmt的格式把一个时间字符串解析为时间元组.

time.time()
	返回当前时间的时间戳(1970纪元后经过的浮点秒数).

time.tzset()
	根据环境变量TZ重新初始化时间相关设置.
'''
