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

#include "utils.h"

#include <log4z.h>

int main_center_server(int argc, char** argv) {
	lw_int32 port = 19800;

	SocketServer serv;

	CenterServerHandler *servHandler = new CenterServerHandler();
	serv.listenHandler = SOCKET_LISTENER_SELECTOR_2(CenterServerHandler::onSocketListener, servHandler);

// 	serv.disConnectHandler = SOCKET_EVENT_SELECTOR(CenterServerHandler::onSocketDisConnect, servHandler);
// 	serv.timeoutHandler = SOCKET_EVENT_SELECTOR(CenterServerHandler::onSocketTimeout, servHandler);
// 	serv.errorHandler = SOCKET_EVENT_SELECTOR(CenterServerHandler::onSocketError, servHandler);
// 
// 	serv.parseHandler = SOCKET_PARSE_SELECTOR_4(CenterServerHandler::onSocketParse, servHandler);

	if (serv.create(new SocketConfig("0.0.0.0", port))) {
		serv.serv([port](lw_int32 what) {
			LOGFMTD("center server running. [port:%d]", port);
		});

		while (1) { lw_sleep(10); }
	}

	return 0;
}
