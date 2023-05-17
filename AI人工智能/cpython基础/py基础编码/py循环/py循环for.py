#coding=utf-8



# 
# 1.for 写法:
#
# 1.1 for 循环迭代list列表 [折叠写法]
fruits=["apple", "banana", "cherry"]
for x in fruits:
	print(x)



# 1.2 for 循环迭代单串字符串(逐个字母迭代)
pstr="love you idiot"
for x in pstr:
	print(x)





#
# 2.for 循环四个关键字: break, continue, else, pass
#
plist=[0,0,0,0,1,2,3,4,5,6,7,8,9,0,0,0,0]
tmp=5

for x in plist:
	# 出现少于0 的数字, break 中断退出
	if(tmp < 0):
		break

	if(x == 0):
		pass

	if(x >= 5):
		print(x,tmp)
		tmp-=1
		continue

else:
	print("否则for 结束时, 会触发for-else.")
	print("如果因为break 而退出for, 不会触发for-else;")
	pass





#
# 3.range() 自动生成
#
# 3.1: range() 默认从0开始, 包括0, 但不包括10. [0,10)
for x in range(10):
	print(x),
else:
	print("for x in range(10): end.")

# 3.2: range(第一参数=起始, 第二参数=结束, 第三参数=递加数值, 可以为负数);
for x in range(10,0,-1):
	print(x),
else:
	print("for x in range(10,0,-1): end.")





#
# 4.for 循环嵌套
#
for x in range(10):
	for y in range(10):
		print("f"),
	#for end
#for end
