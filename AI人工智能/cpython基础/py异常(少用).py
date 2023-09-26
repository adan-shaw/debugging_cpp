#coding=utf8


#
# python 异常类&debug说明
#
'''
	python 本身就是所见即所得的脚本, 并不需要太多调试技巧, 你只需要知道哪一行报错即可,
	所以其实不太需要异常报错的.
	在大规模server 场合, 报错基本也是看daemon log 日志的.
	你也可以将daemon server的标准io, 导入到一个文件中, 进而记录标准io 输出, 方便查找错误.
'''




#
# *** 异常demo ***
#
try:
	# 分母为0, 算术出错(会触发异常的);
	1 / 0;


# 使用常规异常捕获类, 去捕捉执行过程中的异常
except Exception as e:
	# 返回字符串类型的异常信息
	#print(e);
	print(str(e));

	# 给出较全的异常信息
	#print(repr(e));


	# 需要导入traceback模块, 获取最全的异常信息(基本不用)
	#print(traceback.print_exc());
	#print(traceback.format_exc());


else:
	print("没有异常, 代码运行正常");
	exit(None);


finally:
	print("最后总要执行我");
	quit(None);





'''
	***
	*** python标准异常类 ***
	***
	BaseException									所有异常的基类
	SystemExit										解释器请求退出
	KeyboardInterrupt							用户中断执行(通常是输入^C)
	Exception											常规错误的基类
	StopIteration									迭代器没有更多的值
	GeneratorExit									生成器(generator)发生异常来通知退出
	StandardError									所有的内建标准异常的基类
	ArithmeticError								所有数值计算错误的基类
	FloatingPointError						浮点计算错误
	OverflowError									数值运算超出最大限制
	ZeroDivisionError							除(或取模)零 (所有数据类型)
	AssertionError								断言语句失败
	AttributeError								对象没有这个属性
	EOFError											没有内建输入,到达EOF 标记
	EnvironmentError							操作系统错误的基类
	IOError												输入/输出操作失败
	OSError												操作系统错误
	WindowsError									系统调用失败
	ImportError										导入模块/对象失败
	LookupError										无效数据查询的基类
	IndexError										序列中没有此索引(index)
	KeyError											映射中没有这个键
	MemoryError										内存溢出错误(对于Python 解释器不是致命的)
	NameError											未声明/初始化对象 (没有属性)
	UnboundLocalError							访问未初始化的本地变量
	ReferenceError								弱引用(Weak reference)试图访问已经垃圾回收了的对象
	RuntimeError									一般的运行时错误
	NotImplementedError						尚未实现的方法
	SyntaxError										Python 语法错误
	IndentationError							缩进错误
	TabError											Tab 和空格混用
	SystemError										一般的解释器系统错误
	TypeError											对类型无效的操作
	ValueError										传入无效的参数
	UnicodeError									Unicode 相关的错误
	UnicodeDecodeError						Unicode 解码时的错误
	UnicodeEncodeError						Unicode 编码时错误
	UnicodeTranslateError					Unicode 转换时错误
	Warning												警告的基类
	DeprecationWarning						关于被弃用的特征的警告
	FutureWarning									关于构造将来语义会有改变的警告
	OverflowWarning								旧的关于自动提升为长整型(long)的警告
	PendingDeprecationWarning			关于特性将会被废弃的警告
	RuntimeWarning								可疑的运行时行为(runtime behavior)的警告
	SyntaxWarning									可疑的语法的警告
	UserWarning										用户代码生成的警告



	BaseException
		+-- SystemExit (exit() = 正常退出python, 带异常... quit() = 强制退出python, 不管异常)
		+-- KeyboardInterrupt
		+-- GeneratorExit
		+-- Exception (可见Exception 真的已经足够了)
			+-- StopIteration
			+-- StopAsyncIteration
			+-- ArithmeticError
			|		+-- FloatingPointError
			|		+-- OverflowError
			|		+-- ZeroDivisionError
			+-- AssertionError
			+-- AttributeError
			+-- BufferError
			+-- EOFError
			+-- ImportError
			|		+-- ModuleNotFoundError
			+-- LookupError
			|		+-- IndexError
			|		+-- KeyError
			+-- MemoryError
			+-- NameError
			|		+-- UnboundLocalError
			+-- OSError
			|		+-- BlockingIOError
			|		+-- ChildProcessError
			|		+-- ConnectionError
			|		|		+-- BrokenPipeError
			|		|		+-- ConnectionAbortedError
			|		|		+-- ConnectionRefusedError
			|		|		+-- ConnectionResetError
			|		+-- FileExistsError
			|		+-- FileNotFoundError
			|		+-- InterruptedError
			|		+-- IsADirectoryError
			|		+-- NotADirectoryError
			|		+-- PermissionError
			|		+-- ProcessLookupError
			|		+-- TimeoutError
			+-- ReferenceError
			+-- RuntimeError
			|		+-- NotImplementedError
			|		+-- RecursionError
			+-- SyntaxError
			|		+-- IndentationError
			|				 +-- TabError
			+-- SystemError
			+-- TypeError
			+-- ValueError
			|		+-- UnicodeError
			|				 +-- UnicodeDecodeError
			|				 +-- UnicodeEncodeError
			|				 +-- UnicodeTranslateError
			+-- Warning
					 +-- DeprecationWarning
					 +-- PendingDeprecationWarning
					 +-- RuntimeWarning
					 +-- SyntaxWarning
					 +-- UserWarning
					 +-- FutureWarning
					 +-- ImportWarning
					 +-- UnicodeWarning
					 +-- BytesWarning
					 +-- ResourceWarning
'''
