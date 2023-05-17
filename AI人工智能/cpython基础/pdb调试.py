#coding=utf-8



#
# python pdb 调试说明
#
'''
	pdb 目前是python 的内建模块, 不需要从pip 中安装, 直接import pdb; 即可调用pdb 模块;
	pdb 的两种装载*.py 文件的方式:
			# 装载py 文件(默认模式)
			pdb ./pdb调试.py

			# 最原始的装载方式(指定pdb 模块名):
			python -m pdb ./pdb调试.py



	pdb 用途:
		用来调试运行过程中的python 语句, 打印其他变量的值内容(跟gdb 类似);

	普通python 脚本, 直接所见即所得, 执行到哪一行, 出错会自动终止, 所以pdb 也比较少用;
	惊喜调试, 调试值是否正确的时候, 会用到;



	ps:
		pdb 不能用来调试pyo/pyc 这种已编译过的py代码块;

	python 编译命令:
		python -OO -m py_compile ./pdb调试.py



	pdb 常见的调试命令:
		# 设置断点行号
		b 75

		# 执行continue
		c

		# 函数提前返回return
		r

		# 进入函数step
		s

		# 下一句next
		n

		# 打印变量的值
		pp val


		# 退出pdb
		quit



	# python 程序结束标志(显示这句话, 表示python 程序已结束了)
	#The program finished and will be restarted
'''



import pdb;

val="love you"

# 设置断点(执行暂停, 可以在断点处打印其它的变量值, 观测此时的各个变量的值, 变量值的变化规律)
pdb.set_trace()

print("1")
print(val)
print("3")
print(val)
print("5")


