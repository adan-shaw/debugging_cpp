#include <fstream>
#include <string>
#include <stdio.h>
#include "v3_msg.pb.h"

//encoder path_output = decoder path_input
#define path_output "./pb_bin_v3"

int main(void)
{
	unsigned long count, tmp;
	char name[64] = {0};
	v3proto::MessageArray msg_arr;
	v3proto::Message msg;
	google::protobuf::Map<std::string, v3proto::Message> *msg_map_map = msg_arr.mutable_msg_map();

	std::fstream fs_output(path_output, std::ios::out | std::ios::trunc | std::ios::binary);//打开文件流, 为protobuff 导出做准备



	for (count = 0; count < 10; count++) {
		sprintf(name, "%s%d", "hello", count);
		msg.set_name(name);
		msg.set_hilarity(v3proto::Message_Humour_PUNS);
		msg.set_height_in_cm(123456);
		msg.set_data("world");
		msg.set_result_count(123456);
		msg.set_true_scotsman(true);
		msg.set_score(123.456f);
		for (tmp = 0; tmp < 96; tmp++) {
			msg.add_key(tmp);
		}

		printf("msg size:%d\n", msg.ByteSize());
		(*msg_map_map)[name] = msg;
	}

	printf("msg array size:%d\n", msg_arr.ByteSize());



	if (!msg_arr.SerializeToOstream(&fs_output)) {
		printf("Failed to write msg.\n");
		return -1;
	}

	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}

