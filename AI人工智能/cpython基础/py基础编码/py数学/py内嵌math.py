#coding=utf8



#
# 声明: 只有sum() 是聚合函数, 可以作用于n 个number 数据, (),[],{} 等容器, 都可以做sum() 统计
# 			其他math 数学API, 只能作用于固定的1,2,3 个数据, 不是聚合函数, 否则会报错
#



# python 执行数学表达式:
'''
	eval()表达式函数是一个安全隐患, 绝对不能让用户自己输入表达式, 
	否则会有被注入shell/python 命令的可能, 
	服务端自己确定的输入式, 可以
'''
x=8
print(eval('3 * x + 2 - 1 * 5'))
print(eval('pow(2,2) * 2 / 4 * 9'))
print(eval("x + 4"))



# 绝对值
print(abs(1.1))



#          被除数, 除数; 返回这两个数值的(商, 余数)
print(divmod(5.5, 2))



# 数字最大值(min 同理))
print(max(1.2, 2, 44, 9))

# 字符串最大值=ascii码最大的字符
print(max('1234'))

max_tuple = (1,2,3)
print(max(max_tuple))

max_list = [1.1,2.2,3,0]
print(max(max_list))

max_dict = {'a','b','c','d','e'}
print(max(max_dict))



# 幂运算(支持负数, 不支持浮点, 要使用浮点, 使用math.pow()
print(pow(2,3))
print(2**3)


# 2^3 次方后, 取模(不要用, 好乱, 歧义)
#print(pow(-2,3,5))
print(pow(-2,3) % 5)



# 四舍五入, 第二参数决定保留多少位数
print(round(1.1314926, 1))
print(round(-1.1314926, 5))



# 比较两个未知原始的大小(python3 中不能用):
# 		cmp(x,y);  如果x<y, 返回-1  如果x==y, 返回0  如果x>y, 返回1
#print(cmp(1,2))



# 求2个元素的和(可以直接对各种容器求和, 但参数只能有两个, 对单个容器求和, 第二个参数直接赋值0)
print(sum((-1.5,2.5,3.5) ,4.5 ))
print(sum([1,2,3,4], 0 ))
print(sum({1,2,3,4,5}, 0 ))
#print(sum( 0, {1,2,3,4})) # 顺序不能错



# 排序
print(sorted((-1.5,2.5,3.5)))
print(sorted([1,2,3,4]))
print(sorted({1,2,3,4,5}))


