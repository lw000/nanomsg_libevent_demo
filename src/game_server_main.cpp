// GameServer.cpp
//

#include "main.h"

#if defined(WIN32) || defined(_WIN32)
#include <winsock2.h>
#endif // WIN32

#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <thread>

#include "command.h"
#include "platform.pb.h"

#include "lw_util.h"

#include "client_http.h"

#include "GameServer.h"
#include "net.h"

using namespace LW;

void run_rpc_client(lw_int32 port)
{

}

void signal_handler(int sig)
{
	switch (sig)
	{
	case SIGTERM:
	case SIGINT:
	//case SIGBREAK:
	case SIGABRT:	
		//event_base_loopbreak(__http_base);
		break;
	}
}

int game_server_main(int argc, char** argv)
{
	signal(SIGINT, signal_handler);
	signal(SIGABRT, signal_handler);
	//signal(SIGBREAK, signal_handler);
	signal(SIGTERM, signal_handler);

	SocketInit s;

	lw_int32 port = 0;
	lw_int32 rpc_times = 1;
	lw_int32 http_times = 1;

	GameServer* gServer[1024];

	port = 19801;
	rpc_times = 1;
	http_times = 1;

	for (size_t i = 0; i < rpc_times; i++) {
		DESK_INFO desk_info;
		desk_info.did = i;
		desk_info.max_usercount = 6;
		char buf[64];
		sprintf(buf, "[%d]", i);
		desk_info.name = std::string(buf);
		desk_info.rid = 0;
		desk_info.state = DESK_STATE_Empty;

		GameServer * serv = new GameServer(nullptr);
		if (serv->create(desk_info))
		{
			serv->start("127.0.0.1", port);
		}

		gServer[i] = serv;

		lw_sleep(0.1);
	}

	run_client_http(http_times);

	while (1)
	{
		lw_sleep(1);
	}

	return 0;
}

