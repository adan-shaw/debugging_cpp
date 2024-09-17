#include <iostream>
#include <fstream>
#include <string>
#include "test.pb.h"

#define path_output "./protobuf_file.txt"

int main(void) {
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	test::v2msg_demo pvs_msg;//注意: 此处应该指名namespace = test(在test.proto中指定了)
	pvs_msg.set_m_str("hello,world");
	pvs_msg.set_m_bytes("are you ok?");

	test::v2msg_demo_emsg *pemsg = new test::v2msg_demo_emsg();

	pemsg->set_m_int32(1);
	pemsg->set_m_str("embedded_Info");
	pvs_msg.set_allocated_m_emsg(pemsg);

	pvs_msg.add_repeated_int32(2);
	pvs_msg.add_repeated_int32(3);
	pvs_msg.add_repeated_str("repeated1");
	pvs_msg.add_repeated_str("repeated2");

	//序列化到file stream
	std::fstream fs_output(path_output, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!pvs_msg.SerializeToOstream(&fs_output)) {
		std::cerr << "Error: " << std::endl;
		return -1;
	}

	//序列化到string
	std::string str_output;
	try {
		pvs_msg.SerializeToString(&str_output);
	}catch (const std::system_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return -1;
	}
	std::cout << str_output << std::endl;

	pemsg->Clear();
	pvs_msg.Clear();
	//delete pemsg;//protobuff 不能直接delete 类实体, protobuff 使用Clear() 清除msg中的数据, 并释放其占用的资源
	return 0;
}
