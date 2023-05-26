#coding=utf-8

# python 是否需要对称加密算法?
'''
	一般情况下, web 应用都不需要对称加密算法, 直接用https 即可, 安全可靠;
	一般情况下, app 游戏等小应用, 也不需要对称加密算法;
	少数app python-server, 对安全性要求很高的场合, 可以引入python 对称加密算法, 此处省略;
'''

# python 标准库, 有标准的ssl 类, 可以轻松实现ssl 加密传输网络数据;
