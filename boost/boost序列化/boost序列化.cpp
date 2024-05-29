//编译:
//		g++ -g3 ./boost序列化.cpp -l boost_serialization -o x

//boost 序列化的弊端
/*
	boost 序列化的优势, 可以说很低!!
	首先json/xml 都是纯文本库, 跨平台能力基本上都是最强的, 完全没有太多平台依赖, 基本上有编译器, 就能编译,
	这种场合, boost 序列化想要替代json/xml 基本上是没有可能的,
	因此, boost 序列化, 只会用在一些特殊场合, 例如你想自定义一套新的序列化规则, 则可以用Boost(但真的不建议你这么做)



	boost 相对于json/xml 的劣势:
		* 字符编码格式的问题: 
				win/unix 的字符编码格式是不统一的, win 一般个宽字符uchat(16bit), unix 一般是窄字符uchat(8bit);
				boost 想要跨平台, 就必须先解决字符编码格式的问题, 但貌似boost 序列化库, 没有看到相关的实现(可能是我水平低, 没看清楚, 暂时不敲定)
				[ps: 就算boost 解决了这个问题, 还有更多麻烦事]

		* 换行符号的不同:
				win/unix 换行符号, 是有差异的, win=/r/n, unix=/n;
				这种不同的换行符号, 会导致text 文本出现致命错误!!
				(部分低级editor/编译器, 是识别不了这种换行符差异, 导致很多unknow bug!! 
				 高级editor/编译器, 偶尔也会出错!!)

		* 性能差, 操作复杂, 缺乏易用性(必须先安装Boost 序列化库, 才能用, 但boost 库安装条件较为苛刻):
				这几点, 不用我细说了, 自己品品, 肯定是json/xml 方便快捷性能高!! 根本没有可比性

		综上所述:
			如果你有特殊的序列化设计需求, 则可以用boost 序列化库试试, 
			如果没有特殊的序列化设计需求, json/xml 能够胜任的情况下, 用boost 作甚??
*/

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>


struct MyRecord{
public:
	std::string a;
	std::string b;
	//为使当前的结构能支持序列化, 得加入下面的代码段
private:
	friend class boost::serialization::access;
	template < class Archive > void serialize (Archive & ar, const unsigned int version)
	{
		ar & a;
		ar & b;
	}
};

struct MyData{
public:
	MyData (){}
	MyData (int id, std::string strName, std::string strValue)
	{
		this->id = id;
		this->strName = strName;
		this->strValue = strValue;
	}
	int id;
	std::string strName;
	std::string strValue;
	std::vector < MyRecord > listMR;
private:
	friend boost::serialization::access;			//声明友元, 授予访问权限
	template < typename Archive > void serialize (Archive & ar, const unsigned int version)//序列化函数
	{
		ar & id;
		ar & strName & strValue;
		ar & listMR;
	}
};

void printTest (MyData & mt)
{
	std::cout << "a=" << mt.id << "\n" << "uid=" << mt.strName.c_str () << "\n" << "usr=" << mt.strValue.c_str () << std::endl;
	std::vector < MyRecord >::iterator iter = mt.listMR.begin ();
	while (iter != mt.listMR.end ())
	{
		std::cout << iter->a.c_str () << "  " << iter->b.c_str () << std::endl;
		iter++;
	}
}

int main (void)
{
	//序列化
	std::stringstream ss;
	MyData p1 (11, "anderson", "neo");				//被序列化的对象
	MyRecord mr, mr2;													//为被序列化对象添加两条记录
	mr.a = "apple", mr.b = "第一条记录";
	mr2.a = "this is b", mr2.b = "第二条记录";
	p1.listMR.push_back (mr);
	p1.listMR.push_back (mr2);

	boost::archive::text_oarchive (ss) << p1;	//执行boost 序列化->ss 字符串流
	std::cout << "序列化后的string 字符串:\n" << ss.str () << std::endl;;

	//序列化为xml形式要求中文为utf-8编码, 否则打开文件是乱码
	//std::stringstream ss;
	//std::ofstream ofs("./a.xml");
	//boost::archive::xml_oarchive oa(ofs);
	//oa << BOOST_SERIALIZATION_NVP(p1);

	//反序列化
	MyData p2;																//被反序列化的对象
	boost::archive::text_iarchive (ss) >> p2;	//反序列化

	printTest (p2);														//打印反序列后的数据
	return 0;
}



