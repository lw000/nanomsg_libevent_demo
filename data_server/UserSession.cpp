#include "UserSession.h"

#include "socket_processor.h"
#include "socket_config.h"

#include "log4z.h"


UserSession::UserSession()
{
	this->reset();
}

void UserSession::reset()
{
}

UserSession::~UserSession() {
	this->destroy();
}

int UserSession::create(SocketProcessor* processor, SocketConfig* conf, evutil_socket_t fd) {
	int c = SocketSession::create(SESSION_TYPE::server, processor, conf, fd);
	
	return c;
}

void UserSession::destroy() {

	SocketSession::destroy();
}