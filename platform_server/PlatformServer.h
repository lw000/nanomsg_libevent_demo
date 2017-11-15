#ifndef __PlatformServer_ServerHandler_h__
#define __PlatformServer_ServerHandler_h__

#include "socket_server.h"
#include "UserServer.h"
#include "RoomServerMgr.h"
#include "lock.h"

class PlatformServerHandler : public SocketServer
{
public:
	RoomServerMgr _roomserver;

public:
	int _base_client_id;
	lw_fast_mutex _lock;

public:
	PlatformServerHandler();
	virtual ~PlatformServerHandler();

public:
	void loadConfig();

public:
	SocketSession* onSocketListener(SocketProcessor* processor, evutil_socket_t fd);
	void onSocketListenerError(void * userdata, int er);

public:
	void onSocketDisConnect(SocketSession* session);
	void onSocketTimeout(SocketSession* session);
	void onSocketError(SocketSession* session);

public:
	int onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize);
};

#endif	// !__PlatformServer_ServerHandler_h__
