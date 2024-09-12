#include <iostream>
#include <fstream>
#include <string>
#include "test.pb.h"



int main(void) {
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	test::pbuff_msg pff_msg;//注意: 此处应该指名namespace = test(在test.proto中指定了)
	pff_msg.set_m_str("hello,world");
	pff_msg.set_m_bytes("are you ok?");

	test::pbuff_msg_em_msg *emsg2 = new test::pbuff_msg_em_msg();

	emsg2->set_m_int32(1);
	emsg2->set_m_str("embeddedInfo");
	pff_msg.set_allocated_emsg(emsg2);

	pff_msg.add_repeated_int32(2);
	pff_msg.add_repeated_int32(3);
	pff_msg.add_repeated_str("repeated1");
	pff_msg.add_repeated_str("repeated2");

	//序列化到file
	std::string filename = "./protobuf_file.txt";
	std::fstream output(filename, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!pff_msg.SerializeToOstream(&output)) {
		std::cerr << "Error: " << std::endl;
		return -1;
	}

	//序列化到string
	std::string str_output;
	try {
		pff_msg.SerializeToString(&str_output);
	}catch (const std::system_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return -1;
	}

	delete emsg2;
	return 0;
}
