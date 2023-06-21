//
// 声明:
//
字符串处理库, 还是主要用纯c 来处理, 
c++ 主要做逻辑耦合即可,
c++ 主要做容器封装, 用低级c++ 即可, 不能大量应用c++ 特性.
因为这样的话, 程序的精度, 可靠性, 稳定性, 跨平台能力都会下跌.



使用时string 类, 注意:
	一个std::string 变量, 只能有一串字符串, 一个'\0' 结束字符!!
	缓冲区是自动分配的, 也是自动剪裁大小的.

	无论你有什么样的编程思路, 都不应该将2 串字符串, 放到1 个std::string 变量中.
	这是原则问题!!









//
//c++ 窄string 类成员函数
//
//1.constructors构造函数 / destructor析构函数:
basic_string()	构造函数, 一般不需要显示调用;
								负责初始化string类, 但不会预先分配c_str() buf.

~basic_string()	析构函数, 一般不需要显示调用;
								负责释放string类, 如果已申请了c_str() buf, 则释放buf; 如果没有, 简单结束

get_allocator()	返回关联的allocator_type分配器
operator=				'='号运算符重载, 将字符串拷贝到string->buf中
assign()				花式赋值,多种方式格式化赋值字符串,最终还是拷贝唯一一串string->buf中


//2.operators 操作符重载:
operator=
operator+
operator==
operator!=
operator<
operator>
operator<=
operator>=


file:///home/msata128/svn2/http_docu_lib/cpp_cn/reference/zh/cpp/string/basic_string.html



//3.API list:
append()     在str的末尾, 插入添加另一串str
assign()     为字符串赋新值
at()         按给定索引值返回字符
begin()      返回一个迭代器, 指向第一个字符
c_str()      将字符串以C字符数组的形式返回
capacity()   返回重新分配空间前的字符容量
compare()    比较两个字符串
copy()       将内容复制为一个字符数组
data()       返回内容的字符数组形式
empty()      如果字符串为空, 返回真
end()        返回一个迭代器, 指向字符串的末尾. (最后一个字符的下一个位置)
erase()      删除字符
find()       在字符串中查找字符
find_first_of()     查找第一个与value中的某值相等的字符
find_first_not_of() 查找第一个与value中的所有值都不相等的字符
find_last_of()      查找最后一个与value中的某值相等的字符
find_last_not_of()  查找最后一个与value中的所有值都不相等的字符
get_allocator()     返回配置器
insert()     插入字符
length()     返回字符串的长度
max_size()   返回字符的最大可能个数
rbegin()     返回一个逆向迭代器, 指向最后一个字符
rend()       返回一个逆向迭代器, 指向第一个元素的前一个位置
replace()    替换字符
reserve()    保留一定容量以容纳字符串(设置capacity值)
resize()     重新设置字符串的大小
rfind()      查找最后一个与value相等的字符(逆向查找)
size()       返回字符串中字符的数量
substr()     返回某个子字符串
swap()       交换两个字符串的内容


