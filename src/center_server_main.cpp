//============================================================================
// Name        : main.cpp
// Author      : lw
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "CenterServer.h"

#include "main.h"

#include "socket_config.h"
#include "socket_server.h"
#include "socket_processor.h"

#include "utils.h"
#include "net.h"

#include <log4z.h>
using namespace zsummer::log4z;

int main_center_server(int argc, char** argv) {
	SocketInit sinit;

	lw_int32 port = 19800;

	SocketServer serv;
	if (serv.create(new CenterServerHandler(), new SocketConfig("0.0.0.0", port))) {
		serv.serv([port](lw_int32 what) {
			LOGFMTD("center server running. [port:%d]", port);
		});

		while (1) { lw_sleep(1); }
	}

	return 0;
}
