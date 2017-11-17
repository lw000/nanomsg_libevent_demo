//============================================================================
// Name        : main.cpp
// Author      : lw
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "main.h"

#include <nn.h>
#include <reqrep.h>
#include "log4z.h"
#include <thread>

#include "common_type.h"
#include "nanomsgcpp_socket.h"
#include "message_queue.h"

using namespace zsummer::log4z;

struct pthread_args {
		int destroy_flag;
		int connection_flag;
};

struct push_pthread_args {
		struct pthread_args *pargs;
		std::string url;
};

class RepServer: public NanomsgcppSocket {
	public:
		RepServer() {
		}

		virtual ~RepServer() {
		}

	public:
		virtual void onRecv(lw_int32 cmd, char* buf, lw_int32 bufsize)
				override {

		}
};

class ReqClient: public NanomsgcppSocket {
	public:
		ReqClient() {
		}

		virtual ~ReqClient() {

		}

	private:
		virtual void onRecv(lw_int32 cmd, char* buf, lw_int32 bufsize)
				override {
			switch (cmd) {
				case 100: {
					printf("msg:%s\n", buf);
				}
					break;
				default:
					break;
			}
		}
};

int main_reqrep_servr(int argc, char** argv) {
	if (argc < 3)
		return 0;
	return 0;
}
