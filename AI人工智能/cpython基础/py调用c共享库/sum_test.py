# -*- coding: utf8 -*-

import ctypes

# 装载*.so 动态链接库
so = ctypes.CDLL('./sum.so')



def display_dict():
	print( "\nType of so is %s" % type(so) )
	print( "\nAttributes before calling so.sum: %s" % dir(so) )
	print( "\nso.sum(10) = %s" % so.sum(10) )
	print( "\nAttributes after calling so.sum: %s" % dir(so) )

def py_sum(x):
	y = 0
	for i in range(x+1):
		y += i
	return y

def so_sum(x):
	return so.sum(x)



if __name__ == "__main__":
	display_dict()
