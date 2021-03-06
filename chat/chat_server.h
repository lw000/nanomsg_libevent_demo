#ifndef __ChatServer_H__
#define __ChatServer_H__

#include <UserMgr.h>
#include <string>
#include <vector>

#include "common_type.h"
#include "common_struct.h"
#include "socket_client.h"
#include "GameDeskHandler.h"
#include "lock.h"
#include "socket_server.h"

class SocketProcessor;
class SocketSession;

class ChatServerHandler : public SocketServer
{
private:
	UserMgr _users;
	int _client_id;
	lw_fast_mutex _lock;

public:
	ChatServerHandler();
	virtual ~ChatServerHandler();

public:
	bool create(SocketConfig* conf);
	void destroy();

public:
	SocketSession* onSocketListener(SocketProcessor* processor, evutil_socket_t fd);
	void onSocketListenerError(void * userdata, int er);

public:
	void onSocketDisConnect(SocketSession* session);
	void onSocketTimeout(SocketSession* session);
	void onSocketError(SocketSession* session);
	int onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize);
};

#endif	//__ChatServer_H__

