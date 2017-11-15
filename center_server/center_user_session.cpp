#include "center_user_session.h"
#include "socket_processor.h"
#include "socket_config.h"

#include "log4z.h"

#include "command.h"
#include "platform.pb.h"

using namespace LW;

CenterUserSession::CenterUserSession()
{
	this->reset();
}

void CenterUserSession::reset()
{

}

CenterUserSession::~CenterUserSession() {
	this->destroy();
}

int CenterUserSession::create(SocketProcessor* processor, SocketConfig* conf, evutil_socket_t fd) {
	int c = SocketSession::create(SESSION_TYPE::server, processor, conf, fd);
	
	return c;
}

void CenterUserSession::destroy() {

	SocketSession::destroy();
}


void CenterUserSession::onSocketDisConnect(SocketSession* session)
{

}

void CenterUserSession::onSocketTimeout(SocketSession* session)
{
}

void CenterUserSession::onSocketError(SocketSession* session)
{
}

int CenterUserSession::onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
{
	switch (cmd)
	{
	case cmd_heart_beat:
	{
		platform::msg_heartbeat msg;
		msg.set_time(time(NULL));
		lw_int32 c = (lw_int32)msg.ByteSize();
		std::unique_ptr<char[]> s(new char[c + 1]);
		lw_bool ret = msg.SerializeToArray(s.get(), c);
		if (ret)
		{
			session->sendData(cmd_heart_beat, s.get(), c);
		}

		break;
	}
	default: {

		break;
	}
	}

	return 0;
}
