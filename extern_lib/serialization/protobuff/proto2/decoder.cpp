#include <fstream>
#include <string>
#include <stdio.h>
#include "v2_msg.pb.h"

//encoder path_output = decoder path_input
#define path_input "./pb_bin_v2"

int main(void)
{
	int tmp;
	v2proto::MessageArray msg_arr;
	std::fstream fs_input(path_input, std::ios::in | std::ios::binary);//打开文件流, 为protobuff 导入做准备

	if (!msg_arr.ParseFromIstream(&fs_input)) {
		printf("Failed to parse %s.\n", path_input);
		return -1;
	}

	//google::protobuf::Map<std::string, v2proto::Message> _msg_map = msg_arr.msg_map();
	google::protobuf::Map<std::string, v2proto::Message>::const_iterator it = msg_arr.msg_map().begin();
	for (; it != msg_arr.msg_map().end(); it++) {
		printf("key:%s\n", it->first.c_str());
		printf("  msg.name:%s\n", it->second.name().c_str());
		printf("  msg.hilarity:%u\n", it->second.hilarity());
		printf("  msg.height_in_cm:%u\n", it->second.height_in_cm());
		printf("  msg.data:%s\n", it->second.data().c_str());
		printf("  msg.result_data:%ld\n", it->second.result_count());
		printf("  msg.true_scotsman:%d\n", it->second.true_scotsman());
		printf("  msg.score:%f\n", it->second.score());
		printf("  msg.score num:%u\n", it->second.key().size());
		for (tmp = 0; tmp < it->second.key().size(); tmp++) {
			printf("  msg.score[%d]:%lu\n", tmp, it->second.key(tmp));
		}
	}

	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}

