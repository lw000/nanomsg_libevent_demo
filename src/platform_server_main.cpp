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


std::string __s_center_server_addr;
lw_ushort16 __s_center_server_port;

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
	if (argc < 2) return 0;

	{
		// 	std::thread a(_add_user_thread);
		// 	std::thread b(_remove_user_thread);
		// 	a.detach();
		// 	b.detach();
	}

	{
		// 	int create_times = 10000000;
		// 	{
		// 		clock_t t = clock();
		// 		for (size_t i = 0; i < create_times; i++)
		// 		{
		// 			NetHead head;
		// 			NetMessage* msg = NetMessage::create(&head);
		// 			if (nullptr != msg)
		// 			{
		// 				NetMessage::release(msg);
		// 			}
		// 		}
		// 		clock_t t1 = clock();
		// 		printf("NetMessage create[%d] : %f, %f\n", create_times, ((double)t1 - t) / CLOCKS_PER_SEC, (((double)t1 - t) / CLOCKS_PER_SEC) / create_times);
		// 	}
	}

	SocketInit s;

	SocketProcessor::processorUseThreads();

	ILog4zManager::getInstance()->start();

	__s_center_server_port = 19800;

	lw_int32 port = 19801;

	if (__g_serv.create(new PlatformServerHandler()))
	{
		__g_serv.run(port, [](int what)
		{
			printf("platform server running. [%d]\n", __g_serv.getPort());

			__connect_center_server(__s_center_server_addr.c_str(), __s_center_server_port);
		});

		while (1) { lw_sleep(1); }
	}

	return 0;
}

