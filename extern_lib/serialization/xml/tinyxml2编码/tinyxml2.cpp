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



	declaration = doc.NewDeclaration();								//新建xml doc声明
	doc.InsertFirstChild(declaration);								//将xml doc声明插入头部

	root=doc.NewElement("Response");									//新建元素, 并设置元素名为: Response
	doc.InsertEndChild(root);													//将元素插入xml_doc插入尾部

	type = doc.NewElement("Type");										//新建元素, 并设置元素名为: Type
	type->SetText("success");													//设置元素的内容
	type->SetAttribute("id","1234");									//设置元素的属性
	root->InsertEndChild(type);												//将元素Type 设置为元素Response 的子元素

	doc.Print(&printer);															//将xml_doc 的数据, 打印(保存)到Xmlprint类中
	str = printer.CStr();															//printer.CStr() 转换成const char*类型
	std::cout << printer.CStr() << std::endl;					//printer.CStr() 直接打印到stdout

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

	root = doc.RootElement();													//获取xml root 树根

	// 遍历打印root xml根 begin (不支持三层结构, 只支持两层结构, 太复杂真的不好定义, 深度太深, 太麻烦!!)
	printf("<%s>\n", root->Name());
	son = root->FirstChildElement();									//获取第一个子元素
	while(son){
		printf("<%s", son->Name());											//打印子元素的name

		attr = son->FirstAttribute();										//获取子元素的属性
		if(attr != NULL){
			do{
				printf(" %s=%s",attr->Name(),attr->Value());//打印子元素的名字, 值
			}while(attr->Next());
		}
		printf(">");

		son_child = son->FirstChildElement();						//获取子元素的第一个孙元素
		while(son_child){																//遍历子元素的所有孙元素
			printf("<%s", son_child->Name());							//打印孙元素的name

			attr = son_child->FirstAttribute();						//获取孙元素的属性
			if(attr != NULL){
				do{
					printf(" %s=%s",attr->Name(),attr->Value());//打印孙元素的名字, 值
				}while(attr->Next());
			}
			printf(">");

			content = son_child->GetText();
			printf("%s", content);												//打印孙元素的值

			printf("</%s>\n", son_child->Name());

			son_child = son_child->NextSiblingElement();	//下一个孙元素
		}
		printf("</%s>\n", son->Name());
		son = son->NextSiblingElement();								//下一个子元素
	}
	printf("</%s>\n", root->Name());
	// 遍历打印root xml根 end

	return ;
}



int main(void){
	cre_xml_data_dom();
	parse_xml_dom(xml_filename);
	return 0;
}





