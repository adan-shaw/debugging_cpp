#coding=utf8



# 打印[0,100] 中的偶数, 中途遇到50的时候break 退出(不会经过while-else)
tmp=0



# while 循环: 条件为真, 则继续执行
while(tmp < 100):
	if(tmp%2 == 0):
		print(tmp),

	if(tmp%2 != 0):
		pass

	if(tmp == 50):
		break

	tmp+=1
	continue

else:
	print("\nwhile-else: while 已经结束")
	print("break 语句不会进入while-else")
#while end
