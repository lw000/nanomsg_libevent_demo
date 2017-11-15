#ifndef __CenterUserSession_h__
#define __CenterUserSession_h__

#include <string.h>

#include "common_type.h"
#include "socket_session.h"
#include "socket_session.h"

#include <event2/util.h>

#include "common_struct.h"

class SocketProcessor;
class SocketConfig;

class CenterUserSession : public SocketSession
{
public:
	UserInfo userinfo;

public:
	CenterUserSession();
	virtual ~CenterUserSession();

public:
	int create(SocketProcessor* processor, SocketConfig* conf, evutil_socket_t fd);
	void destroy();

public:
	void reset();

public:
	void onSocketDisConnect(SocketSession* session);
	void onSocketTimeout(SocketSession* session);
	void onSocketError(SocketSession* session);

public:
	int onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize);
};

#endif	// !__CenterUserSession_h__
