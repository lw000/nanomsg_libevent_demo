// main.cpp :
//

#include "main.h"

#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <thread>
#include <vector>
#include <algorithm>

#include "command.h"
#include "platform.pb.h"

//#include "http_server.h"

#include "PlatformServer.h"

#include "client_main.h"

#include "utils.h"
#include "Users.h"

#include "socket_config.h"
#include "socket_processor.h"

#include <log4z.h>
#include "socket_hanlder.h"

using namespace LW;

static Users			__g_umgr;

static void _add_user_thread()
{
	srand(time(NULL));
	while (1)
	{
		for (int i = 0; i < 50000; i++)
		{
			USER_INFO info;
			info.uid = /*rand() + */10000000 + i;
			__g_umgr.add(info, nullptr);
		}
// 		lw_sleep(1);
	}
}

static void _remove_user_thread()
{
	while (1)
	{
		for (int i = 0; i < 10000; i++)
		{
			int uid = rand() + 10000000;
			//__g_umgr.removeUserTest();
			__g_umgr.remove(uid);
		}
// 		lw_sleep(1);
	}
}

int main_platform_server(int argc, char** argv)
{
//	{
//		std::thread a(_add_user_thread);
//		std::thread b(_remove_user_thread);
//		a.detach();
//		b.detach();
//	}

	lw_int32 port = 19801;

	SocketServer serv;
	PlatformServerHandler *servHandler = new PlatformServerHandler();
	serv.listenHandler = SOCKET_EVENT_SELECTOR(PlatformServerHandler::onListener, servHandler);
	serv.connectedHandler = SOCKET_EVENT_SELECTOR(PlatformServerHandler::onSocketConnected, servHandler);
	serv.disConnectHandler = SOCKET_EVENT_SELECTOR(PlatformServerHandler::onSocketDisConnect, servHandler);
	serv.timeoutHandler = SOCKET_EVENT_SELECTOR(PlatformServerHandler::onSocketTimeout, servHandler);
	serv.errorHandler = SOCKET_EVENT_SELECTOR(PlatformServerHandler::onSocketError, servHandler);
	
	serv.parseHandler = SOCKET_PARSE_SELECTOR_4(PlatformServerHandler::onSocketParse, servHandler);
	
	if (serv.create(new SocketConfig("0.0.0.0", port)))
	{
		serv.serv([port](int what)
		{
			LOGFMTD("platform server running. [port:%d]", port);
		});

		while (1) { lw_sleep(1); }
	}

	return 0;
}

