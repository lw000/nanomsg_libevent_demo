#ifndef __ChatUserSession_h__
#define __ChatUserSession_h__

#include <string.h>

#include "common_type.h"
#include "socket_session.h"
#include "socket_session.h"

#include <event2/util.h>

#include "common_struct.h"

class SocketProcessor;
class SocketConfig;

class ChatUserSession : public SocketSession
{
public:
	UserInfo userinfo;

public:
	ChatUserSession();
	virtual ~ChatUserSession();

public:
	int create(SocketProcessor* processor, SocketConfig* conf, evutil_socket_t fd);
	void destroy();

public:
	void reset();
};

#endif	// !__ChatUserSession_h__
