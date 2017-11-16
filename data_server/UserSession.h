#ifndef __UserSession_h__
#define __UserSession_h__

#include <string.h>

#include "common_type.h"
#include "socket_session.h"
#include "socket_session.h"

#include <event2/util.h>

#include "common_struct.h"

class SocketProcessor;
class SocketConfig;

class UserSession: public SocketSession {
	public:
		UserInfo userinfo;

	public:
		UserSession();
		virtual ~UserSession();

	public:
		int create(SocketProcessor* processor, SocketConfig* conf,
		evutil_socket_t fd);
		void destroy();

	public:
		void reset();
};

#endif	// !__UserSession_h__
