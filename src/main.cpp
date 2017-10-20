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

#include <time.h>

#ifdef __cplusplus 
extern "C" {
#endif
#include "queue.h"
#ifdef __cplusplus
}
#endif

#include "socket_processor.h"

#include "net.h"
#include "log4z.h"
#include "utils.h"
#include "Threadable.h"


using namespace zsummer::log4z;

static queue_t * __g_queue;

struct lw_base_data
{
	char t;
};

struct lw_base_data_int
{
	char t;
	int x;
};

int main(int argc, char** argv) {
	if (argc < 2)
	{
		printf("please input server name/n");
		return 0;
	}
	
	START_ENTER_METHOD METHODS[] = {
		{ "center", main_center_server },
		{ "platform", main_platform_server },
		{ "game", main_game_server},
		{ "http_server", main_http_server },
		{ "pubsub", main_pubsub_servr },	//pubsub tcp://127.0.0.1:5555 -s
		{ "survey", main_nanomsg_survey },
		{ "nanorpc", main_nonamsg_rpc },
		{ "ai", main_ai_server },
		{ "timer", main_timer_server },
		{ "http_client", main_http_client },
	};

	std::string s(argv[1]);

	SocketInit sinit;

	ILog4zManager::getInstance()->start();

	SocketProcessor::use_threads();

	//event_enable_debug_logging

	__g_queue = queue_init(1024);
	for (int i = 0; i < 100; i++) {
		queue_add(__g_queue, new char('1'));
	}

	for (int i = 0; i < 100; i++) {
		queue_add(__g_queue, new int(1000));
	}

	for (int i = 0; i < 100; i++) {
		queue_add(__g_queue, new double(i));
	}

	struct ss {
		int x;
		int y;
		ss(int x, int y): x(x), y(y) {
			
		}
	};
	
	for (int i = 0; i < 100; i++) {
		queue_add(__g_queue, new ss(i, i+1));
	}

	for (int i = 0; i < 1024; i++) {
		char* p = (char*)queue_remove(__g_queue);

		delete p;
	}
	
	queue_dispose(__g_queue);

	srand(time(NULL));

	for (int i = 0; i < sizeof(METHODS) / sizeof(METHODS[0]); i++) {
		if (s.compare(METHODS[i].name) == 0) {
			
			if (METHODS[i].call != NULL) {
				METHODS[i].call(argc, argv);
			}

			break;
		}
	}

	return 0;
}
