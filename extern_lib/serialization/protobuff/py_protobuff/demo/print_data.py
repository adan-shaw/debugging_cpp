# -*- coding: utf-8 -*-

# 导入proto 生成的*.py 文件(类似include c++中自动生成的protobuff 头文件)
import data_pb2
import json

if(__name__ == "__main__"):
	test_dict =  {
		"test1":1,
		"test2":2
	}

	# 创建demo_data 数据载体
	demo_data = data_pb2.demo_data()

	# 填充demo_data 的内容
	demo_data.request_type = 1
	demo_data.tool_type = 1
	# 把dict json 序列化后, 当成字符串传给protobuff 的string 字符串
	demo_data.data = json.dumps(test_dict)

	# 打印原始的demo_data 
	print(demo_data)

	# 导出demo_data 序列化后的字符串
	encode = demo_data.SerializeToString()
	# 打印序列化后的demo_data 
	print(encode)


	# 创建一个空的demo_data 载体
	demo_data_empty = data_pb2.demo_data()

	# 不需要填充demo_data_empty 的内容

	# 执行反序列化, 填充demo_data_empty 的内容
	demo_data_empty.ParseFromString(encode)

	# 打印demo_data_empty
	print(demo_data_empty)

	# demo_data_empty 的引用
	print(demo_data_empty.request_type)
	print(demo_data_empty.tool_type)
	print(demo_data_empty.data)


