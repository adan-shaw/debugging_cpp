#coding=utf8



# 固定参数类型, 固定返回值类型, 只有python3 才有!!
# 另外: python 中没有判断宏, 所以在函数定义上, 没办法兼容python2/3!!
# 		 (运行前会做检查, 发现语法定义错误, 禁止运行)



# 固定形参类型(python3 only)
def val_type_confirm(try_int: int, try_str: str = "default value") -> str:
	print(try_int)
	print(try_str)
	return "return a string"
print(val_type_confirm(999))



# 固定参数为容器: dict/list/tuple
def val_type_confirm(try_dict: dict, try_list: list, try_tuple: tuple) -> str:
	print(try_dict)
	print(try_list)
	print(try_tuple)
	return "return a string"
print(val_type_confirm({11,22},[11,22],(11,22)))











