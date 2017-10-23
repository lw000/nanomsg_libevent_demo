// GameServer.cpp
//

#include "main.h"

#include <stdio.h>
#include <iostream>
#include <signal.h>
#include "utils.h"
#include "net.h"
#include "socket_config.h"
#include "GameClient.h"
#include "GameClientHandler.h"
#include "log4z.h"

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

	DeskInfo desk_info;
	desk_info.did = 0;
	desk_info.max_usercount = 2;
	desk_info.name = "²âÊÔ";
	desk_info.rid = 0;
	desk_info.state = DESK_STATE_Empty;

	GameClientHandler* cliHandler = new GameClientHandler(desk_info);
	GameClient * cli = new GameClient(cliHandler);
	if (cli->create(new SocketConfig("127.0.0.1", 19801)))
	{

	}

	while (1) {
		lw_sleep(1);
	}

	return 0;
}

