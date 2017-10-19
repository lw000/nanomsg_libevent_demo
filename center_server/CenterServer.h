#ifndef __CenterServer_ServerHandler_h__
#define __CenterServer_ServerHandler_h__

#include "socket_server.h"
#include "session_manager.h"

class CenterServerHandler
{
public:
	SessionManager Sessions;

public:
	CenterServerHandler();
	virtual ~CenterServerHandler();

public:
	SocketSession* onSocketListener(SocketProcessor* processor, evutil_socket_t fd);

public:
	void onSocketDisConnect(SocketSession* session);
	void onSocketTimeout(SocketSession* session);
	void onSocketError(SocketSession* session);

public:
	void onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize) /*override*/;
};

#endif	// !__CenterServer_ServerHandler_h__
