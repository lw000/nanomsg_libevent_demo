//============================================================================
// Name        : main.cpp
// Author      : lw
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "main.h"

#include <string.h>
#include <stdio.h>
#include <string>
#include <unordered_map>

#include <time.h>

#include "socket_processor.h"

#include "net.h"
#include "log4z.h"
#include "utils.h"
#include "Threadable.h"
#include "cache_queue.h"

using namespace zsummer::log4z;

#define TEST_CACHE_BUF_SIZE 512

struct ss {
	int x;
	int y;
	ss(int x, int y) :
			x(x), y(y) {

	}

	~ss() {

	}
};

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("please input server name/n");
		return 0;
	}

	ThreadableC11 t;
		t.start(
				[]() {
					NewCacheQueue q(64*1024);
					std::string s("1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM");
					q.push(s.c_str(), s.size());
					char*c = q.front();
					char buf[TEST_CACHE_BUF_SIZE + 1];
					buf[TEST_CACHE_BUF_SIZE] = '\0';
					q.copyto(buf, TEST_CACHE_BUF_SIZE);
					q.pop();
					q.pop(5);
					q.clear();
				});


	SocketInit sinit;

	ILog4zManager::getInstance()->start();

	SocketProcessor::use_threads();

	//event_enable_debug_logging

	srand(time(NULL));
	std::unordered_map<std::string, MAIN_FUNC> methods =
		{
				{ "center", main_center_server },
				{ "platform", main_platform_server },
				{"game", main_game_server },
				{ "http_server", main_http_server },
				{ "pubsub", main_pubsub_servr },	//pubsub tcp://127.0.0.1:5555 -s
				{ "survey", main_nanomsg_survey },
				{ "nanorpc", main_nonamsg_rpc },
				{ "ai", main_ai_server },
				{ "timer", main_timer_server },
				{"http_client", main_http_client },
				{"db", main_db_server },
		};

	//	methods["center"] = main_center_server;
	//	methods["platform"] = main_platform_server;
	//	methods["game"] = main_game_server;
	//	methods["http_server"] = main_http_server;
	//	methods["pubsub"] = main_pubsub_servr;	//pubsub tcp://127.0.0.1:5555 -s
	//	methods["survey"] = main_nanomsg_survey;
	//	methods["nanorpc"] = main_nonamsg_rpc;
	//	methods["ai"] = main_ai_server;
	//	methods["timer"] = main_timer_server;
	//	methods["http_client"] = main_http_client;

	std::string s(argv[1]);
	std::unordered_map<std::string,MAIN_FUNC>::iterator m = methods.find(s);
	if (m != methods.end()) {
		m->second(argc, argv);
	}

	return 0;
}
