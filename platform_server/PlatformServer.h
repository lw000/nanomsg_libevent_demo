#ifndef __PlatformServer_ServerHandler_h__
#define __PlatformServer_ServerHandler_h__

#include "socket_server.h"
#include "UserManager.h"
#include "lock.h"

class PlatformServerHandler
{
public:
	UserManager _users;

public:
	int _base_client_id;
	lw_fast_lock _lock;

public:
	PlatformServerHandler();
	virtual ~PlatformServerHandler();

public:
	SocketSession* onSocketListener(SocketProcessor* processor, evutil_socket_t fd);

public:
	void onSocketDisConnect(SocketSession* session);
	void onSocketTimeout(SocketSession* session);
	void onSocketError(SocketSession* session);

public:
	int onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize);
};



#endif	// !__PlatformServer_ServerHandler_h__
