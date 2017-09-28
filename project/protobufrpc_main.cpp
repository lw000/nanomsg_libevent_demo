#include "main.h"

#include "platform.pb.h"
#include "RpcServer.hh"
#include "RpcChannel.hh"
#include <iostream>
#include <signal.h>
#include <algorithm>

using namespace platform;
using namespace nrpc;

#define ECHO_ENDPOINT_PORT "tcp://127.0.0.1:7777"

class EchoServiceImpl : public MyService {
public:
	EchoServiceImpl() {}
	virtual void Echo(::google::protobuf::RpcController* controller,
		const ::EchoReqMsg* request,
		::EchoRespMsg* response,
		::google::protobuf::Closure* done) {

		int a = request->a();
		int b = request->b();
		response->set_c(a + b);
		if (done) {
			done->Run();
		}
	}
};

static int _rpc_client() {
	try {

		nrpc::RpcChannel rpc_channel(ECHO_ENDPOINT_PORT);
		platform::MyService_Stub::Stub stub(&rpc_channel);
		//while (1)
		{ 
			{
			 	platform::EchoReqMsg request;
			 	platform::EchoRespMsg response;
			 	request.set_a(10);
			 	request.set_b(10);
			 	stub.Echo(NULL, &request, &response, NULL);
			 	std::cerr << response.c() << std::endl;
			}

		}
	}
	catch (nn::exception& e) {
		std::cerr << "NN EXCEPTION : " << e.what() << std::endl;
	}
	catch (std::exception& e) {
		std::cerr << "STD EXCEPTION : " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << " UNTRAPPED EXCEPTION " << std::endl;
	}

	return 0;
}

static int _rpc_server() {
	EchoServiceImpl *impl = new EchoServiceImpl();
	RpcServer rpc_server(ECHO_ENDPOINT_PORT);
	rpc_server.RegisterService(impl);
	rpc_server.Start();
	return 0;
}

static void OnExit(int sig)
{
	std::cerr << "Exiting on ^C " << sig << std::endl;
}

int protobufrpc_main(int argc, char *argv[]) {

	signal(SIGINT, OnExit);

	std::string s(argv[2]);

	if (s.compare("-s") == 0) {
		_rpc_server();
	}
	else if (s.compare("-c") == 0) {
		_rpc_client();
	}
	else {
		printf("please input reqrep -s or reqrep -c. \n");
	}
	return 0;
}
