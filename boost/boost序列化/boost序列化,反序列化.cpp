//编译:
//		g++ -g3 ./boost序列化,反序列化.cpp -l boost_serialization -o x



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



