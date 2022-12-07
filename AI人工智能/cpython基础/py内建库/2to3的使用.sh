#!/bin/sh



# 2to3 工具存放路径(shell 变量名不能是数字开头的, 否则报错: 变量未定义)
py2to3="/home/tarball/cpython2/bin/2to3"

# 2to3 之后, py3 导出时, 保存的路径
py3_output="./py3_output"



# 生成测试数据样板:
:<<!
	py2:
		print "fuck you"

	py3(实际上python2.6/2.7 也支持这种print("") 方式, 因此, 此乃python 通用打印标准):
		print("fuck you")
!

# 创建测试数据:
echo "print \"fuck you\"" > test.py







# 1.显示2to3 之后的代码, 不保存, 只打印到屏幕上;
# 	[不能用重定向将echo 打印输出保存下来, 作为新的py3 文件, 这种方式会出错的;
# 	 所以, 这个功能, 只做查看, 不做转换]
$py2to3 test.py





# 2.导出2to3 之后, 生成的代码(-w 覆盖源文件), 
# 	默认参数: -f all(默认转换全部fixers额外的固定器)
#$py2to3 -w test.py
# 	等价于:
#$py2to3 -f all -w test.py


# 导出2to3 之后, 不覆盖原文件, 导出到新文件中(输出文件夹, 不存在会自动创建)
if [ ! -d $py3_output ]; then
	mkdir $py3_output
fi
$py2to3 -w test.py -n -o $py3_output





# 3.如果在你的Python文件里有doctests, 你还需要带-d选项来转换doctests, 
# 	并且, 如果你有doctests本文text, 你还需要显示地指明这些文件, 再执行.
#$py2to3 -w .
#$py2to3 -w -d .
#$py2to3 -w -d src/README.txt
#$py2to3 -w -d src/tests/*.txt




