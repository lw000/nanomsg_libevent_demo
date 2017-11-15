#ifndef __CenterServer_ServerHandler_h__
#define __CenterServer_ServerHandler_h__

#include "socket_server.h"
#include "session_manager.h"

class CenterServerHandler : public SocketServer
{
public:
	SessionManager Sessions;

public:
	CenterServerHandler();
	virtual ~CenterServerHandler();

public:
	SocketSession* onSocketListener(SocketProcessor* processor, evutil_socket_t fd);
	void onSocketListenerError(void * userdata, int er);
};

#endif	// !__CenterServer_ServerHandler_h__
