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

SimpleMessageQueue __g_msg_queue;

class PushServer : public NanomsgcppSocket
{
public:
	PushServer() {
	}

	virtual ~PushServer() {
	}

public:
	virtual void onRecv(lw_int32 cmd, char* buf, lw_int32 bufsize) override {

	}
};


class SubClient : public NanomsgcppSocket
{
public:
	SubClient() {
	}

	virtual ~SubClient() {
	}

private:
	virtual void onRecv(lw_int32 cmd, char* buf, lw_int32 bufsize) override {
		switch (cmd)
		{
		case 100: {
			printf("msg:%s\n", buf);
		} break;
		default:
			break;
		}
	}
};


/*  The client runs in a loop, displaying the content. */

int sub_thread_server(const char *url)
{
	SubClient client;

	int fd;
	fd = client.create(AF_SP, NN_SUB);

	if (fd < 0) {
		return (-1);
	}

	if (client.connect(url) < 0) {
		return (-1);
	}

	/*  We want all messages, so just subscribe to the empty value. */

	if (client.setsockopt(NN_SUB, NN_SUB_SUBSCRIBE, "", 0) < 0) {
		return (-1);
	}

	do
	{
		int bytes = client.recv();
		if (bytes < 0 && nn_errno() == ETIMEDOUT) break;
		if (bytes < 0) {
			fprintf(stderr, "recv fail: %sn", nn_strerror(errno));
			break;
		}
	} while (1);

	return (-1);
}


/*  The server runs forever. */

void pub_thread_server(void* args)
{
	struct push_pthread_args *pargs = (struct push_pthread_args *)args;

	PushServer serv;
	int fd = serv.create(AF_SP, NN_PUB);
	if (fd < 0)
	{
		return;
	}

	if (serv.bind(pargs->url.c_str()) < 0)
	{
		return;
	}

	printf("pub server start ...\n");

	pargs->pargs->connection_flag = 1;

	for (;;)
    {
		if (!__g_msg_queue.empty())
		{
			Msgdata msg = __g_msg_queue.pop();
			{
				char s[512];
				sprintf(s, "pop size [%lld]", __g_msg_queue.size());
				LOGD(s);
			}
			serv.send(msg.getmtype(), msg.getmtext(), msg.getmtextl());
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	serv.shutdown();

	return;
}

static void *pthread_push_msgdata(void *args)
{
	struct pthread_args *pargs = (struct pthread_args *)args;

	//* waiting for connection with server done.*/
	while (!pargs->connection_flag)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	while (!pargs->destroy_flag)
	{
		const char* d = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

		Msgdata dmsg(100, (unsigned char*)d, strlen(d));
		{
			__g_msg_queue.push(dmsg);
		}

   		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return nullptr;
}

int main_pubsub_servr(int argc, char** argv) {
	if (argc < 3) return 0;

	ILog4zManager::getInstance()->start();

	__g_msg_queue.createChannel();

	struct pthread_args* pargs = new struct pthread_args;
	pargs->connection_flag = 0;
	pargs->destroy_flag = 0;

	{
		std::thread a(pthread_push_msgdata, pargs);
		a.detach();
	}

	struct push_pthread_args* push_args = new struct push_pthread_args;
	push_args->pargs = pargs;
	push_args->url = argv[2];

	if (strcmp(argv[3], "-s") == 0) {
		std::thread a(pub_thread_server, push_args);
		a.detach();
	}
	else if (strcmp(argv[3], "-c") == 0) {
		std::thread a(sub_thread_server, argv[2]);
		a.detach();
	} else {
		fprintf(stderr, "usage: %s <url> [-s]\n", argv[0]);
	}

	while (1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	return 0;
}
