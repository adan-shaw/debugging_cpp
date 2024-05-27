//编译:
//		g++ -ltinyxml2 ./tinyxml2.cpp -o x 

//安装依赖:
//		apt-get install libtinyxml2-6a libtinyxml2-dev libtinyxml2.6.2v5 libtinyxml2.6.2v5-dbg

//刷新.so 共享库list:
//		sudo ldconfig



#include <iostream>
#include <string.h>
#include <cstring>
#include <tinyxml2.h>

using namespace tinyxml2;



#define xml_filename "./test_data"
#define test_count (10000)



//1.生成一棵的xml 树, 然后将该xml 树导出到file 中保存
void cre_xml_data_dom(void){
	XMLDocument doc;
	XMLDeclaration *declaration;
	XMLElement* root,*type;
	XMLPrinter printer;
	std::string str;



	declaration = doc.NewDeclaration();							//新建xml doc声明
	doc.InsertFirstChild(declaration);							//将xml doc声明插入头部

	root=doc.NewElement("Response");								//新建元素
	doc.InsertEndChild(root);												//将元素插入xml_doc插入尾部

	type = doc.NewElement("Type");
	type->SetText("success");
	root->InsertEndChild(type);

	doc.Print(&printer);														//将xml_doc 的数据, 打印(保存)到Xmlprint类中
	str = printer.CStr();														//printer.CStr() 转换成const char*类型
	std::cout<<printer.CStr()<<std::endl;						//printer.CStr() 直接打印

	return;
}



//2.解析一个xml 文件, 将解析到的结果打印出来
void parse_xml_dom(const char* xml_file_path){
	XMLDocument doc;
	XMLElement *root,*son,*son_child;
	const XMLAttribute *attr;
	const char* content;
	XMLError err;



	err = doc.LoadFile("test.xml");
	if(err != XML_SUCCESS){
		printf("doc.LoadFile() error!!");
		return;
	}

	root = doc.RootElement();												//获取xml root 树根

	son = root->FirstChildElement("node");					//获取第一个子元素

	while(son){
		attr = son->FirstAttribute();									//获取该子元素的属性
		printf("%s:%s\n",attr->Name(),attr->Value());	//打印该子元素的名字and 值

		son_child = son->FirstChildElement();					//获取该子元素的第一个孙元素
		while(son_child){															//遍历该子元素的孙元素
			content = son_child->GetText();
			printf("%s\n", content);										//打印孙元素的值

			son_child = son_child->NextSiblingElement();//下一个孙元素
		}
		son = son->NextSiblingElement();							//下一个子元素
	}

	return ;
}



int main(void){
	cre_xml_data_dom();
	parse_xml_dom(xml_filename);
	return 0;
}





