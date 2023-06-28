//编译:
//		g++ -std=c++17 -g3 ./c++17-path.cpp -o x -lstdc++fs



//1.filesystem 文件系统差异:
/*
	文件系统提供对保存为文件(file)的永久信息的访问机制, 但不同系统中文件系统的属性和操纵文件系统的方式差异巨大;
	下面简单介绍Microsoft Windows和POSIX的差异:
	1.Windows 支持多个根名称, 例如:
			# disk 磁盘根
			c:
			d:
			e:
			f:
			...
			# 网络根
			\\network_name;
			...
		Windows 文件系统由一个多根树组成的, 
		并且每个树都有其自己的当前目录(即有n 个根, 就有n 个当前目录)
		获取相对路径的时候, 会显示根头部名;

		POSIX支持单个树(无'根名', 不会有c:, d: e: f:), 统一根名为: \
		仅支持一个当前目录;



	2.路径名的本机表示方式:
		Windows 使用以null 结尾的序列wchar_t, 编码为utf-16(每个acter)一个或多个元素char;
		POSIX 使用以null 结尾的序列char, 编码为utf-8(每个acter)的一个或多个元素char;

		类的对象 path 以本机形式存储路径名, 但支持在此存储的窗体和多个外部窗体之间轻松转换:
			以null 值结束的序列char, 编码为操作系统所优先的;
			以null 结尾的序列char, 编码为utf-8;
			以null 值结束的序列wchar_t, 编码为操作系统所优先的;
			以null 结尾的序列char16_t, 编码为utf-16;
			以null 结尾的序列char32_t, 编码为utf-32;

		通过使用一个或多个codecvt facet, 按需调节这些表示形式之间的相互转换;
		如果未指定特定的区域设置对象, 则将从全局区域设置获取这些facet;



	3.操作系统允许你获取指定'文件/目录'访问权限的详细信息:
		Windows 记录文件是只读/可写, 这对于目录没有意义;
		POSIX 记录文件是否可以读取、写入、执行的(方便'文件/目录'属主做各种权限管理操作)
*/



//2.c++17 支持std::filesystem 文件路径库, 可以屏蔽win 和unix 系统之间的差异;
/*
API list:
	void copy(const path& from, const path& to):                        目录复制
	path absolute(const path& pval, const path& base = current_path()): 获取相对于base的绝对路径
	bool create_directory(const path& pval):                            当目录不存在时创建目录
	bool create_directories(const path& pval):                          形如/a/b/c这样的, 如果都不存在, 创建目录结构
	bool exists(const path& pval):                                      用于判断path是否存在
	uintmax_t file_size(const path& pval):                              返回目录的大小
	file_time_type last_write_time(const path& pval):                   返回目录最后修改日期的file_time_type对象
	bool remove(const path& pval):                                      删除目录
	uintmax_t remove_all(const path& pval):                             递归删除目录下所有文件, 返回被成功删除的文件个数
	void rename(const path& from, const path& to):                      移动文件或者重命名

编译选项:
	-lstdc++fs
	ps:
		由于c++17 是新增的库, 想要编译c++17 的代码, 也是通过.so 共享链接库的方式新增的, 
		所以必须加-l 链接库选项参数, 否则编译会找不到API;
*/



#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;									//重命名namespace, 并启用该namespace
using std::string;
using std::cout;
using std::endl;



int main(void){
	//string str = R"(./c++17-path.cpp)";					//*** 字符串原始字面量, 简化了我们在使用regex库时正则表达式的书写 ***
	//fs::path path_str = str;
	fs::path path_str = "./c++17-path.cpp";				//fs::path 路径初始化


	fs::path path = fs::current_path();						//fs::path 返回值API
	cout << "current_path() = " << path << endl;	//返回当前工作目录的绝对路径(可以不用path.c_str() 来返回字符串, 但推荐用)

	fs::path parent_path = path.parent_path();		//返回当前工作目录的父路径

	if(fs::exists(path_str))											//询问路径是否存在
		cout << path_str.c_str() << " 存在" << endl;	//打印fs::path 的路径字符串
	else
		cout << path_str.c_str() << " 不存在" << endl;


	if(path.has_root_path())											//fs::path 存在root 目录路径, 则返回root 目录路径
		cout << "root_path() = " << path.root_path() << endl;

	if(path.has_root_name())											//fs::path 存在root 目录name, 则返回root 目录name(linux 无根名, win 是: c: d: e: f:)
		cout << "root_name() = " << path.root_name() << endl;

	if(path.has_root_directory())									//fs::path 存在root 目录, 则返回root 目录
		cout << "root_directory() = " << path.root_directory() << endl;


	if(path_str.has_relative_path())							//返回相对路径
		cout << "relative_path() = " << path_str.relative_path() << endl;

	cout << "absolute_path() = " << absolute(path_str) << endl;//相对路径, 转换为绝对路径

	if(path_str.has_filename())										//返回文件名
		cout << "filename() = " << path_str.filename() << endl;

	if(path_str.has_stem())												//返回主干路径
		cout << "stem() = " << path_str.stem() << endl;

	if(path_str.has_extension())									//返回文件扩展名路径
		cout << "extension() = " << path_str.extension() << endl;


	fs::current_path(path.parent_path());					//修改当前工作目录为: .. (父目录)

	if(path.has_parent_path())										//返回父目录路径
		cout << "parent_path() = " << path.parent_path() << endl;

	return 0;
}



