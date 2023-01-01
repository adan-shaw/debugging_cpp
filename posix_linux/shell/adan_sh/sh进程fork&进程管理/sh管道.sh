#!/bin/sh

# 谨记:
# 	管道传递, 一定是标准输出1, 不一定是标准输出2 !!
# 	所以, if [ "`id $user_name 2>/dev/null | grep "uid="`"  != "" ];then
# 	做字符串输出传递, 判断时, 取stdout, 不取stderr!!

# 	if [] 时, 尽量保留stdout-1 作为判断依据, 不要用stderr-2, 
# 	否则可能if 结果总是为""空, 因为管道传输, 总是依赖stdout-1




#
# 1.管道用在读取文件上(无缺陷, 不会删掉空格和换行, 但默认容量只有64kb)
#
file_path="sh管道.sh"
if [ -f $file_path ]; then
	cat $file_path | while read line
	do
		echo $line
	done
fi





#
# 2.在执行echo回显结果的基础上, 再执行二次.exe 程序.
#
find . -name "*.js" -exec rm {} \;

mkdir test_path
find . -name "*.js" -exec mv {} ./test_path/ \;
rm -r test_path





#
# 3.给准备执行的.exe 程序, 传入执行参数.
#
# 用管道传xargs参数, -t表示在执行之前先打印:
echo "file1 file2 file3" | xargs -t touch
# 等价于:
touch file1 file2 file3


# -n选项, 每一个参数, 都让touch 执行一次!! 直到所有参数都用完为止.
echo "file1 file2 file3" | xargs -n1 -t touch
# 等价于:
touch file1 
touch file2 
touch file3

rm file1 file2 file3





#
# 4.用管道来判断.exe 执行结果!!
#
:<<!
注意:
	管道的默认容量一般是64kb, 超出容量, 一般都会有问题.

注意2:
	管道会将1,2 标准输出, 标准错误, 都传递到下一条命令中.
	test测试:
		id ssss | grep "no such user"
		id ssss 1>/dev/null | grep "no such user"
		id ssss 2>/dev/null | grep "no such user"
	因此, 你需要知道你想要的是标准输出1, 还是标准错误2


注意3:
	$(), `` 这种反引命令, 会像管道一样, 标准输出1,2 都装载.
	但是不建议使用!!
	tmp=$(), tmp=``, 是很不靠谱的.
	tmp变量只能装载一句字符串!!
	而且就算装载进去, 也会删除所有空格和换行.
	所以判断命令执行结果, 最好还是用管道, 或者重定向到文件, 再做判断.
!






