#coding=utf8



import sys



# 初始化标准输出API(提高引用效率)
stdout=sys.stdout.write
stdin=sys.stdin.readline
stderr=sys.stderr.write



# 标准输出API的使用
stdout("hello world\n")
stderr("hello world\n")

stdout("请输入一行(readline):\n")
tmp=stdin()
stdout(tmp)
