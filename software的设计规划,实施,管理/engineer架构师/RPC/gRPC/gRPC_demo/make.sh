#!/bin/sh

# 使用protobuf 工具生成C++ 代码(--cpp_out=. --grpc_out=. 表明: 自动生成的rpc 代码, 都放在当前文件夹下, '=.'是指代输出路径)
protoc --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` service.proto



# 编译选项-lgrpc -lgrpc++ -lprotobuf (注意, c++ 和c 语言的grpc 编译选项是不一样的)
# 其他编译选项: -lgrpc++_reflection -lgrpc -lgrpc++ -labsl_synchronization
g++ -g3 service.pb.cc service.grpc.pb.cc client.cpp -o client -lgrpc -lgrpc++ -lprotobuf
g++ -g3 service.pb.cc service.grpc.pb.cc server.cpp -o server -lgrpc -lgrpc++ -lprotobuf

#g++ -g3 client.cpp -o client -lgrpc -lprotobuf
#g++ -g3 server.cpp -o server -lgrpc -lprotobuf
