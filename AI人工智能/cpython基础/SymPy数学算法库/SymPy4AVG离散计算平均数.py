#coding=UTF-8

import time
import array
import math

# 使用离散的方式, 算平均数, 方差, 标准差



# 计算机离散算法, 和数学公式不太一样; 计算机需要存储, 需要另外的方法进行计算



def func():
	count = 10240;
	while(count > 0):
		count-=1;

# 计算func() 函数的执行效率
def func_math_takes_time():
	start = time.clock()
	func()
	end = time.clock()
	return end-start



def main():
	# 执行func() 1024 次, 计算: 平均值, 方差, 标准差(使用数据容器, 存储数据, 实现: 化离散为连续[当然这是一个最蠢的办法, 有更聪明的办法])
	test_count = 1024

	sum = 0.0
	count = 0
	data=array.array('f')
	while(count < test_count):
		data.insert(count, func_math_takes_time())
		sum += data[count]
		count+=1;

	# 计算平均数
	avg = sum/test_count

	# 计算方差
	count = 0
	variance_sum = 0.0
	while(count < test_count):
		# 方差和
		variance_sum += math.pow((data[count]-avg),2)
		count+=1;

	# 方差
	variance = variance_sum / test_count

	# 标准差(等价公式: math.pow(variance，0.5) = math.sqrt(variance))
	#math.pow(variance，0.5)
	std_deviation = math.sqrt(variance)

	# 打印结果
	print("test_count = ", test_count)
	print("avg = ", avg)
	print("variance_sum = ", variance_sum)
	print("variance = ", variance)
	print("std_deviation = ", std_deviation)



# 执行测试
main()
