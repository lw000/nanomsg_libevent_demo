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

#include "PlatformServer.h"

#include "utils.h"
#include "UserServer.h"

#include "socket_config.h"
#include "socket_processor.h"

#include <log4z.h>
#include "socket_hanlder.h"
#include "UserSession.h"

using namespace LW;

static UserServer __g_umgr;

#define USER_COUNT	10000

static void __on_thread_add_user()
{
	while (1)
	{
		for (int i = 0; i < USER_COUNT; i++)
		{
			UserSession *s = new UserSession;
			s->userinfo.uid = /*rand() + */USER_COUNT + i;
			__g_umgr.add(s);
		}
	}
}

static void __on_thread_remove_user()
{
	while (1)
	{
		int uid = rand() % USER_COUNT;
		__g_umgr.find(uid);
		if (__g_umgr.size() == 0) {
			break;
		}
	}
}

int main_platform_server(int argc, char** argv)
{
// 	{
// 		for (int i = 0; i < USER_COUNT; i++)
// 		{
// 			UserInfo info;
// 			info.uid = i+1;
// 			__g_umgr.add(info, nullptr);
// 		}
// 
// 		//std::thread a(__on_thread_add_user);
// 		std::thread b(__on_thread_remove_user);
// 		//a.detach();
// 		b.detach();
// 	}

	lw_int32 port = 19801;

	PlatformServerHandler serv;
	serv.loadConfig();

	if (serv.create(new SocketConfig("0.0.0.0", port)))
	{
		serv.serv([port](int what)
		{
			LOGFMTD("platform server running. [port:%d]", port);
		});

		while (1) { lw_sleep(10); }
	}

	return 0;
}

