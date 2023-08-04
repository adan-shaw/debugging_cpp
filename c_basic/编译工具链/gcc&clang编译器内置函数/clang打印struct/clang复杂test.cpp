//编译:(clang 编译c++ 程序, 最好加上-lstdc++, 绕开gcc/g++)
//		clang-16 -lstdc++ -g3 ./clang复杂test.cpp -o x



#include <sys/time.h>
#include <string>
#include <string.h>
#include <vector>



class TimeTool{
	public:
		static long long getNowTimeMilliseconds();
		static long long getNowTimeSecond();
		static std::string loaclTime();

	private:
		TimeTool(){};
		~TimeTool(){};
};

long long TimeTool::getNowTimeMilliseconds(){
	struct timeval vl;
	gettimeofday(&vl,NULL);
	/*
	printf("tv_sec(s):%lu \n", vl.tv_sec);
	printf("vl.tv_usec:%lu \n", vl.tv_usec);
	printf("Milliseconds:%lu \n", vl.tv_sec*1000 + vl.tv_usec/1000);
	*/
	return (vl.tv_sec*1000 + vl.tv_usec/1000);
}

long long TimeTool::getNowTimeSecond(){
	struct timeval vl;
	gettimeofday(&vl,NULL);
	return (vl.tv_sec);
}

std::string TimeTool::loaclTime(){
	struct tm* timeinfo;
	char buffer[64];
	time_t mytime = time(&mytime);

	timeinfo = localtime(&mytime);
	strftime(buffer,sizeof(buffer),"[%Y/%m/%d %H:%M:%S] ",timeinfo);
	return std::string(buffer);
}



struct structInfo{
	std::string name;
	std::string type;
	unsigned int arraySize;
};

class structTool{
	public:
		static structTool* getInstances();

		void setStructInfo(const structInfo & info);
		void setStructPtr(void * ptr);

		void printfAll();

		template<class TYPE>
		void createStruct(){
			_ptrHead = new TYPE();
			memset(_ptrHead, 0, sizeof(TYPE));
			_ptrTmp = _ptrHead;
		}

		void releaseStruct();

		void average();

	private:
		structTool(/* args */){ _sInfoVec.reserve(10); };
		~structTool(){ releaseStruct(); };

		void print(const structInfo & info, void **ptr);
		std::string valueToString(const std::string & str, void **ptr);

		template <class T, class T1>
		void addition (T & a ,const T1 & b){
			a += b;
		}

		template<class TYPE>
		void getPtrValue(char * ret, size_t size, char * format, void **ptr){
			if(_isAverage){
				snprintf(ret,size,format,*(TYPE *)*ptr/_index);
				*(TYPE *)*ptr = *(TYPE *)*ptr + sizeof(TYPE);
				return;
			}
			snprintf(ret,size,format,*(TYPE *)*ptr);
			*(TYPE *)*ptr = *(TYPE *)*ptr + sizeof(TYPE);

			addition(*(TYPE *)_ptrTmp,*(TYPE *)_structPtr);
			*(TYPE *)_ptrTmp = *(TYPE *)_ptrTmp + sizeof(TYPE);
		}

		template<class TYPE>
		void ptrNextPlace(void * ptr){
			*(TYPE *)ptr = *(TYPE *)ptr + sizeof(TYPE);
		}

		void printArray(const structInfo & info, void **ptr);

		/* data */
		std::vector<structInfo> _sInfoVec;
		void * _structPtr = nullptr;
		
		void * _ptrHead = nullptr;
		void * _ptrTmp = nullptr;

		uint16_t _index = 0;
		bool _isAverage = false;
	};

structTool* structTool::getInstances(){
	static structTool ins;
	return &ins;
}

void structTool::setStructInfo(const structInfo & info){
	_sInfoVec.emplace_back(std::move(info));
}

void structTool::setStructPtr(void * ptr){
	_structPtr = ptr;
}

void structTool::printfAll(){
	printf("%s \n",TimeTool::loaclTime().c_str());
	for(int i = 0; i < (int)_sInfoVec.size(); ++i)
		print(_sInfoVec[i],&_structPtr);
	printf("\n");
	_ptrTmp = _ptrHead;
	_index++;
}

void structTool::print(const structInfo & info,void **ptr){
	if(info.arraySize == 0)
		printf("%s:%s",info.name.c_str(), valueToString(info.type,ptr).c_str());
	else{
		printf("\n");
		printArray(info,ptr);
		printf("\n");
	}
}

std::string structTool::valueToString(const std::string & str, void **ptr){
	char ret[32];
	if(str == "short")
		getPtrValue<int>(ret,32,"%d, ",ptr);
	else if(str == "int")
		getPtrValue<short>(ret,32,"%d, ",ptr);
	else if(str == "float")
		getPtrValue<float>(ret,32,"%f, ",ptr);
	else if(str == "int64_t")
		getPtrValue<int64_t>(ret,32,"%lld, ",ptr);
	else if(str == "double")
		getPtrValue<double>(ret,32,"%llf, ",ptr);
	else if(str == "long")
		getPtrValue<long>(ret,32,"%lld, ",ptr);
	else if(str == "long long")
		getPtrValue<long long>(ret,32,"%lld, ",ptr);
	else if(str == "char")
		getPtrValue<char>(ret,32,"%s, ",ptr);
	return std::string(ret);
}

void structTool::printArray(const structInfo & info, void **ptr){
	for(int i = 0; i < info.arraySize; ++i)
		printf("%s[%d]:%s",info.name.c_str(),i,valueToString(info.type,ptr).c_str());
}

void structTool::releaseStruct(){
	if(_ptrHead != nullptr)
		delete _ptrHead;
	_ptrHead = nullptr;
	_ptrTmp = nullptr;
	_structPtr = nullptr;
	_sInfoVec.clear();
}

void structTool::average(){
	_isAverage = true;
	printf("%s \n",TimeTool::loaclTime().c_str());
	for(int i = 0; i < (int)_sInfoVec.size(); ++i)
		print(_sInfoVec[i],&_ptrTmp);
	printf("\n");

	_ptrTmp = _ptrHead;
	_index = 0;
	_isAverage = false;
}



struct testInfo{
	float testA[6];
	int   testB;
	int   testC;
	float testD;
	float testE;
	float testF;
	int   testG;
	struct structInfo x;
};



int main(void){
	structTool::getInstances()->createStruct<struct testInfo>();

	structInfo info;
	info.name = "testA";//设置结构体元素的变量名
	info.type = "float";//设置结构体元素的变量类型
	info.arraySize = 6;//如果是array, 则设置元素个数
	structTool::getInstances()->setStructInfo(info);

	info.name = "testB";
	info.type = "int";
	info.arraySize = 0;
	structTool::getInstances()->setStructInfo(info);

	info.name = "testC";
	info.type = "int";
	info.arraySize = 0;
	structTool::getInstances()->setStructInfo(info);

	info.name = "testD";
	info.type = "float";
	info.arraySize = 0;
	structTool::getInstances()->setStructInfo(info);

	info.name = "testE";
	info.type = "float";
	info.arraySize = 0;
	structTool::getInstances()->setStructInfo(info);

	info.name = "testF";
	info.type = "float";
	info.arraySize = 0;
	structTool::getInstances()->setStructInfo(info);

	info.name = "testG";
	info.type = "int";
	info.arraySize = 0;
	structTool::getInstances()->setStructInfo(info);

	testInfo buffer;
	for(int i = 0; i < 5; ++i){
		buffer.testD += 9;
		buffer.testB += 2;
		buffer.testC += 3;
		buffer.testD += 4;
		buffer.testE += 5;
		for(int i = 0; i < 6;++i)
			buffer.testA[i] = i;

		structTool::getInstances()->setStructPtr(&buffer);//设置结构体到类里面
		structTool::getInstances()->printfAll();//打印结构体所有的值
	}

	structTool::getInstances()->average();
	return 0;
}
