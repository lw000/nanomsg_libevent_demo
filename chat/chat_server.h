#ifndef __ChatServer_H__
#define __ChatServer_H__

#include <string>
#include <vector>

#include "common_type.h"
#include "common_struct.h"
#include "socket_client.h"
#include "GameClientHandler.h"
#include "Users.h"
#include "lock.h"

class SocketProcessor;
class SocketSession;

class ChatServerHandler
{
private:
	Users _users;
	int _client_id;
	lw_fast_lock _lock;

public:
	ChatServerHandler();
	virtual ~ChatServerHandler();

public:
	bool create(SocketConfig* conf);
	void destroy();

public:
	SocketSession* onSocketListener(SocketProcessor* processor, evutil_socket_t fd);

public:
	void onSocketConnected(SocketSession* session);
	void onSocketDisConnect(SocketSession* session);
	void onSocketTimeout(SocketSession* session);
	void onSocketError(SocketSession* session);

public:
	void onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize);
};

#endif	//__ChatServer_H__

