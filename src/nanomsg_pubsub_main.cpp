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
#include <pubsub.h>
#include "log4z.h"
#include <thread>

#include "Threadable.h"

#include "common_type.h"
#include "nanomsgcpp_socket.h"
#include "message_queue.h"

using namespace zsummer::log4z;

struct pthread_args {
		int destroy_flag;
		int connection_flag;
};

struct common_pthread_args {
		struct pthread_args *pargs;
		std::string url;
};

SimpleMessageQueue __g_msg_queue;

class PushServer: public NanomsgcppSocket, public Threadable {
	private:
		struct common_pthread_args *thread_data;

	public:
		PushServer(struct common_pthread_args *thread_data) {
			this->thread_data = thread_data;
		}

		virtual ~PushServer() {

		}

	public:
		virtual int onStart() {

			return 0;
		}

		virtual int onRun() {
			int fd = this->create(AF_SP, NN_PUB);
			if (fd < 0) {
				return -1;
			}

			if (this->bind(this->thread_data->url.c_str()) < 0) {
				return -1;
			}

			printf("pub server start ...\n");

			this->thread_data->pargs->connection_flag = 1;

			for (;;) {
				if (!__g_msg_queue.empty()) {
					Msgdata msg = __g_msg_queue.pop();
					{
						char s[512];
						sprintf(s, "pop size [%lld]", __g_msg_queue.size());
						LOGD(s);
					}
					this->send(msg.getmtype(), msg.getmtext(), msg.getmtextl());
				}
				else {
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
			}

			this->shutdown();
			return 0;
		}

		virtual int onEnd() {

			return 0;
		}

	private:
		virtual void onRecv(lw_int32 cmd, char* buf, lw_int32 bufsize)
				override {

		}
};

class SubClient: public NanomsgcppSocket, public Threadable {
	private:
		struct common_pthread_args *thread_data;

	public:
		SubClient(struct common_pthread_args *thread_data) {
			this->thread_data = thread_data;
		}

		virtual ~SubClient() {
		}

	public:
		virtual int onStart() {
			return 0;
		}

		virtual int onRun() {
			int fd;
			fd = this->create(AF_SP, NN_SUB);

			if (fd < 0) {
				return -1;
			}

			if (this->connect(this->thread_data->url.c_str()) < 0) {
				return (-1);
			}

			/*  We want all messages, so just subscribe to the empty value. */

			if (this->setsockopt(NN_SUB, NN_SUB_SUBSCRIBE, "", 0) < 0) {
				return (-1);
			}

			do {
				int bytes = this->recv();
				if (bytes < 0 && nn_errno() == ETIMEDOUT)
					break;
				if (bytes < 0) {
					fprintf(stderr, "recv fail: %sn", nn_strerror(errno));
					break;
				}
			} while (1);
			return 0;
		}

		virtual int onEnd() {
			return 0;
		}

	private:
		virtual void onRecv(lw_int32 cmd, char* buf, lw_int32 bufsize)
				override {
			switch (cmd) {
				case 100: {
					LOGFMTD("%s", buf);
				}
					break;
				default:
					break;
			}
		}
};

static void *pthread_push_msgdata(void *args) {
	struct pthread_args *pargs = (struct pthread_args *) args;

	//* waiting for connection with server done.*/
	while (!pargs->connection_flag) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	while (!pargs->destroy_flag) {
		const char* d =
				"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

		Msgdata dmsg(100, (unsigned char*) d, strlen(d));
		{
			__g_msg_queue.push(dmsg);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	return nullptr;
}

int main_pubsub_servr(int argc, char** argv) {
	if (argc < 3)
		return 0;

	__g_msg_queue.createChannel();

	struct pthread_args pargs;
	pargs.connection_flag = 0;
	pargs.destroy_flag = 0;

	{
		std::thread a(pthread_push_msgdata, &pargs);
		a.detach();
	}

	struct common_pthread_args args;
	args.pargs = &pargs;
	args.url = (argv[3] != NULL) ? argv[3] : "tcp://127.0.0.1:5555";

	PushServer s(&args);
	SubClient c(&args);

	if (strcmp(argv[2], "-s") == 0) {
		s.start();
	}
	else if (strcmp(argv[2], "-c") == 0) {
		c.start();
	}
	else {
		fprintf(stderr, "usage: %s <url> [-s]\n", argv[0]);
	}

	while (1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	return 0;
}
