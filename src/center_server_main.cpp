//============================================================================
// Name        : main.cpp
// Author      : lw
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "CenterServer.h"

#include "main.h"

#include "socket_server.h"
#include "socket_processor.h"

#include "utils.h"
#include "net.h"

#include <log4z.h>
using namespace zsummer::log4z;

static SocketServer		__g_serv;

static void start_cb(lw_int32 what)
{
	char s[512];
	sprintf(s, "center server running. [port: %d]", __g_serv.getPort());
	LOGD(s);
}

int center_server_main(int argc, char** argv) {
// 	if (argc < 2) return 0;

	SocketInit sinit;

	ILog4zManager::getInstance()->start();

	lw_int32 port = 19800;
	if (__g_serv.create(new CenterServerHandler())) {
		__g_serv.listen(port, start_cb);

		while (1) { lw_sleep(1); }
	}

	return 0;
}
