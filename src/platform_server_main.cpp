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

#include "NetPackage.h"
#include "socket_processor.h"
#include "net.h"

#include <log4z.h>
using namespace zsummer::log4z;

using namespace LW;

static Users			__g_umgr;
static SocketServer		__g_serv;

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

int platform_server_main(int argc, char** argv)
{
// 	if (argc < 2) return 0;

	{
		// 	std::thread a(_add_user_thread);
		// 	std::thread b(_remove_user_thread);
		// 	a.detach();
		// 	b.detach();
	}

	SocketInit s;

	ILog4zManager::getInstance()->start();

	if (__g_serv.create(new PlatformServerHandler()))
	{
		__g_serv.listen(19801, [](int what)
		{
			printf("platform server running. [%d]\n", __g_serv.getPort());
		});

		while (1) { lw_sleep(1); }
	}

	return 0;
}

