// GameServer.cpp
//

#include "main.h"

#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <thread>

#include "command.h"
#include "platform.pb.h"
#include "socket_timer.h"
#include "lock.h"
#include "utils.h"

#include "client_http.h"

#include "GameServer.h"

#include "net.h"
#include "socket_processor.h"
#include "socket_session.h"
#include "Threadable.h"
#include "socket_hanlder.h"
#include "socket_config.h"

#include "log4z.h"

using namespace LW;

// void signal_handler(int sig)
// {
// 	switch (sig)
// 	{
// 	case SIGTERM:
// 	case SIGINT:
// 	//case SIGBREAK:
// 	case SIGABRT:	
// 		//event_base_loopbreak(__http_base);
// 		break;
// 	}
// }

int main_game_server(int argc, char** argv) {
// 	signal(SIGINT, signal_handler);
// 	signal(SIGABRT, signal_handler);
// 	signal(SIGTERM, signal_handler);

	DESK_INFO desk_info;
	desk_info.did = 0;
	desk_info.max_usercount = 6;
	desk_info.name = "²âÊÔ";
	desk_info.rid = 0;
	desk_info.state = DESK_STATE_Empty;

	GameServer * serv = new GameServer(nullptr);

	if (serv->create(desk_info))
	{
		serv->start();
	}

// 	run_client_http(http_times);

	while (1) {
		lw_sleep(1);
	}

	return 0;
}

