#include "CenterServer.h"

#include "command.h"
#include "platform.pb.h"

#include "socket_session.h"

#include "log4z.h"
#include "socket_config.h"
#include "server_session.h"
#include "common_marco.h"

using namespace LW;

CenterServerHandler::CenterServerHandler()
{

}

CenterServerHandler::~CenterServerHandler()
{
	
}

SocketSession* CenterServerHandler::onSocketListener(SocketProcessor* processor, evutil_socket_t fd)
{
	ServerSession* pSession = new ServerSession(new SocketConfig);
	pSession->disConnectHandler = SOCKET_EVENT_SELECTOR(CenterServerHandler::onSocketDisConnect, this);
	pSession->timeoutHandler = SOCKET_EVENT_SELECTOR(CenterServerHandler::onSocketTimeout, this);
	pSession->errorHandler = SOCKET_EVENT_SELECTOR(CenterServerHandler::onSocketError, this);
	pSession->parseHandler = SOCKET_PARSE_SELECTOR_4(CenterServerHandler::onSocketParse, this);
	int r = pSession->create(processor, fd);
	if (r == 0)
	{
		Sessions.add(pSession);

		platform::msg_connected msg;
		msg.set_time(time(NULL));
		int len = msg.ByteSize();
		{
			char *s = new char[len];
			bool ret = msg.SerializeToArray(s, len);
			if (ret)
			{
				pSession->sendData(cmd_connected, s, len);
			}
			delete[] s;
		}
	}
	else
	{
		pSession->destroy();
		SAFE_DELETE(pSession);
	}

	return pSession;
}

void CenterServerHandler::onSocketDisConnect(SocketSession* session)
{
	Sessions.remove(session);
}

void CenterServerHandler::onSocketTimeout(SocketSession* session)
{
	Sessions.remove(session);
}

void CenterServerHandler::onSocketError(SocketSession* session)
{
	Sessions.remove(session);
}

int CenterServerHandler::onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
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
