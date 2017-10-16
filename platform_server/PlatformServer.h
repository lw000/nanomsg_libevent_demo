#ifndef __PlatformServer_ServerHandler_h__
#define __PlatformServer_ServerHandler_h__

#include "socket_server.h"
#include "Users.h"
#include "lock.h"

class PlatformServerHandler
{
public:
	Users users;
	lw_fast_lock _lock;

public:
	PlatformServerHandler();
	virtual ~PlatformServerHandler();

public:
	AbstractUser* getUsers();

public:
	int onListener(SocketSession* session);

public:
	int onSocketConnected(SocketSession* session);
	int onSocketDisConnect(SocketSession* session);
	int onSocketTimeout(SocketSession* session);
	int onSocketError(SocketSession* session);

public:
	void onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize);
};



#endif	// !__PlatformServer_ServerHandler_h__
