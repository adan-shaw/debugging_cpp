#coding=UTF-8



# 导入路径检查模块
import os.path



# os.path 路径库
'''
1: 判断path
	os.path.exists(path)             如果path存在, 返回True; (否则返回False)
	os.path.isabs(path)              如果path是绝对路径, 返回True;
	os.path.isfile(path)             如果path是一个存在的文件, 返回True;
	os.path.isdir(path)              如果path是一个存在的目录, 则返回True;

2: path 属性
	os.path.getsize(path)            返回path的大小(字节)
	os.path.getatime(path)           返回path所指向的'文件/目录'的最后存取时间
	os.path.getmtime(path)           返回path所指向的'文件/目录'的最后修改时间

3: path 字符串info
	os.path.abspath(path)            返回path 的绝对路径
	os.path.split(path)              将path 分割为: 目录 & 文件名的tuple, 然后返回tuple
	os.path.dirname(path)            返回path 的路径(等价于os.path.split(path)的第一个元素)
	os.path.basename(path)           返回path 的文件名(等价于os.path.split(path)的第二个元素)
	os.path.splitext(path)           分离文件名&后缀名; 默认返回(file_name,file后缀名)tuple

4: 少用
	os.path.commonprefix(list)       输入一个path list, 返回该apth list 中, 最长的共有path 路径; (例如: /home/adan /home/eva, 共有路径: /home)
	os.path.join(path1,path2,...]])  合并多个路径, 共有部分被去除, 最终返回一个最长路径string;
	os.path.normcase(path)           消除win 系统路径的差异: 在unix下, 该函数会原样返回path; 
																	 在win 平台上会将路径中所有字符转换为小写, 并将所有斜杠转换为反斜杠;
	os.path.normpath(path)           除路径中多余的分隔符和对上级目录的引用, 返回标准化的路径;
	os.path.splitdrive(path)         拆分驱动器名和路径, 主要针对win; linux 返回的元组中, 第一个元素总是空的;
	os.walk(root)                    自动遍历文件夹, 返回比较特殊, 看demo;



5.os 库的目录操作
	os.getcwd()                       获取当前工作目录
	os.chdir("dirname")               改变当前工作目录
	os.symlink('path_src','path_des') 创建符号链接, 源需绝对路径
	os.mkdir('dirname')               创建单级目录
	os.makedirs('dir1/dir2')          可生成多层递归目录(不需要二次mkdir)
	os.rmdir('dirname')               删除单级空目录, 若目录不为空则无法删除, 报错(专门删空目录)
	os.removedirs('dirname1')         自动删除空目录: 若目录为空, 则删除, 并递归到上一级目录, 如若也为空, 则删除, 依此类推;(专门删空目录)
	os.listdir('path')                列出指定目录下的所有文件和子目录, 包括隐藏文件, 并以列表方式打印
	os.walk()                         自动遍历文件夹(py_os.path.py 有demo)


'''





# os.walk() 自动遍历文件夹demo
for root, dirs, files in os.walk("."):
	print("*** root ***")
	print(root)
	print("*** dirs ***")
	print(dirs)
	print("*** files ***")
	print(files)



