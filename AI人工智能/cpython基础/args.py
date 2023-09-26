#coding=utf8

import argparse

if __name__=='__main__':
	# 初始化命令行解析模块
	parser = argparse.ArgumentParser()

	# 一定要添加命令行选项, 否则终止运行(尽量不要占用-h, --help, 默认自带-h, --help)
	parser.add_argument('--url', type=str, default="127.0.0.1")		# 添加str 字符串参数
	parser.add_argument('--port', type=int, default=80)						# 添加int 整形参数

	# 获取命令行参数
	args = parser.parse_args()

	# 打印命令行参数元表
	print(args)

	# 打印命令行参数元表中的元素
	print(args.url)
	print(args.port)
