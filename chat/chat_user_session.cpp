#include "chat_user_session.h"
#include "socket_processor.h"
#include "socket_config.h"

#include "log4z.h"

ChatUserSession::ChatUserSession()
{
	this->reset();
}

void ChatUserSession::reset()
{

}

ChatUserSession::~ChatUserSession() {
	this->destroy();
}

int ChatUserSession::create(SocketProcessor* processor, SocketConfig* conf, evutil_socket_t fd) {
	int c = SocketSession::create(SESSION_TYPE::server, processor, conf, fd);
	
	return c;
}

void ChatUserSession::destroy() {

	SocketSession::destroy();
}
