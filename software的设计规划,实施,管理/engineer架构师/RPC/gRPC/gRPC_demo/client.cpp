#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using example::Greeter;
using example::Greeting;
using example::GreetingResponse;

class GreeterClient {
public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(Greeter::NewStub(channel)) {}

  //同步调用 Greeter 服务
  std::string Greet(const std::string& user) {
    Greeting greeting; //创建一个请求
    greeting.set_name(user);
    GreetingResponse response; //存储回应
    ClientContext context;

    //实际 RPC 调用
    Status status = stub_->Greet(&context, greeting, &response);

    if (status.ok()) {
      return response.message();
    } else {
      std::cerr << "RPC failed." << std::endl;
      return "RPC failed";
    }
  }

private:
  std::unique_ptr<Greeter::Stub> stub_;
};



int main(void) {
  GreeterClient greeter(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = greeter.Greet(user); //发起 RPC 调用
  std::cout << "Greeter received: " << reply << std::endl;
  return 0;
}
