#coding=utf8



import os
import time



# os 库
'''
1.os 库字符串宏
	os.linesep                        输出当前平台使用的'行终止符'(即换行符号), win下为: "\t\n", Linux下为: "\n"
	os.pathsep                        输出驱动器分割符(win 专用, 一般是':')
	os.sep                            输出操作系统特定的路径分隔符, win下为: "\\", Linux下为: "/"
	os.curdir                         返回当前目录符('.')
	os.pardir                         获取当前目录的父目录符('..')

	os.name                           输出字符串指示当前使用平台. win->'nt'; Linux->'posix'
	os.environ                        获取系统环境变量



2.shell
	os.system("bash command")         运行shell命令, 结果直接输出到stdout
	os.popen("bash command")          运行shell命令, 结果保存到python对象中, 可使用readlines() 读出执行结果



3.目录
	os.getcwd()                       获取当前工作目录
	os.chdir("dirname")               改变当前工作目录
	os.symlink('path_src','path_des') 创建符号链接, 源需绝对路径
	os.mkdir('dirname')               创建单级目录
	os.makedirs('dir1/dir2')          可生成多层递归目录(不需要二次mkdir)
	os.rmdir('dirname')               删除单级空目录, 若目录不为空则无法删除, 报错(专门删空目录)
	os.removedirs('dirname1')         自动删除空目录: 若目录为空, 则删除, 并递归到上一级目录, 如若也为空, 则删除, 依此类推;(专门删空目录)
	os.listdir('path')                列出指定目录下的所有文件和子目录, 包括隐藏文件, 并以列表方式打印
	os.walk()                         自动遍历文件夹(py_os.path.py 有demo)



4.文件
	os.access('path',os.W_OK)             检验文件权限模式, 输出True, False(os.X_OK,os.R_OK,os.W_OK)
	os.chmod('path',os.W_OK)              改变文件权限模式
	os.tmpfile()                          创建并打开'w+b'一个新的临时文件
	os.stat('path/filename')              获取文件/目录信息
	os.remove()                           删除一个文件
	os.rename("oldname","newname")        重命名文件/目录
	os.utime('file_name',(before,after))  修改访问时间记录属性
'''



# popen() shell 命令执行
tmp=os.popen("ls")
print(tmp.readlines())

# 自动遍历文件夹
print(os.listdir("."))



# 获取文件的修改时间
mod_time = os.path.getmtime('./py_os.py')
print(mod_time)

# 将修改时间转换为时间戳
mod_timestamp = time.mktime(time.strptime('2020-10-10 10:00:00', '%Y-%m-%d %H:%M:%S'))

# 篡改文件的访问和修改时间(篡改后, gedit 编辑器会显示文件已经修改, 要求你reload)
os.utime('./py_os.py', (mod_timestamp, mod_timestamp))
print(os.path.getmtime('./py_os.py'))


