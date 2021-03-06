// main.cpp :
//

#include "main.h"

#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <UserMgr.h>
#include <thread>
#include <vector>
#include <algorithm>

#include "command.h"
#include "platform.pb.h"

#include "PlatformServer.h"

#include "utils.h"
#include "socket_config.h"
#include "socket_processor.h"
#include "socket_hanlder.h"
#include "chat_server.h"

#include "log4z.h"


using namespace LW;

int main_chat_server(int argc, char** argv)
{
	lw_int32 port = 19802;

	ChatServerHandler serv;
	
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

