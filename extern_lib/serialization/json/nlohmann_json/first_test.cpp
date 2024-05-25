//编译:
//		g++ -g3 -I /home/tarball/nlohmann_json/include ./test.cpp -o x 
//		g++ -g3 -I . ./test.cpp -o x 

#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(void)
{
  auto config_json = json::parse(R"({"happy": true, "pi": 3.141})");//构建json对象
  std::cout << config_json << std::endl;//输出json对象值
  return 0;
}
