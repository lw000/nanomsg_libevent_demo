#include "CenterServer.h"

#include "command.h"
#include "platform.pb.h"

#include "log4z.h"
#include "socket_session.h"
#include "socket_config.h"
#include "server_session.h"
#include "common_marco.h"

using namespace LW;

CenterServerHandler::CenterServerHandler()
{
	this->listenHandler = SOCKET_LISTENER_SELECTOR_2(CenterServerHandler::onSocketListener, this);
	this->listenErrorHandler = SOCKET_LISTENER_ERROR_SELECTOR_2(CenterServerHandler::onSocketListenerError, this);
}

CenterServerHandler::~CenterServerHandler()
{
	
}

SocketSession* CenterServerHandler::onSocketListener(SocketProcessor* processor, evutil_socket_t fd)
{
	ServerSession* pSession = new ServerSession;
	pSession->onDisconnectHandler = SOCKET_EVENT_SELECTOR(CenterServerHandler::onSocketDisConnect, this);
	pSession->onTimeoutHandler = SOCKET_EVENT_SELECTOR(CenterServerHandler::onSocketTimeout, this);
	pSession->onErrorHandler = SOCKET_EVENT_SELECTOR(CenterServerHandler::onSocketError, this);
	pSession->onDataParseHandler = SOCKET_DATAPARSE_SELECTOR_4(CenterServerHandler::onSocketParse, this);
	int r = pSession->create(processor, new SocketConfig, fd);
	if (r == 0)
	{
		Sessions.add(pSession);

		platform::msg_connected msg;
		msg.set_time(time(NULL));
		int len = msg.ByteSize();
		{
			//char *s = new char[len];
			//bool ret = msg.SerializeToArray(s, len);
			std::string s;
			bool ret = msg.SerializeToString(&s);
			if (ret)
			{
				pSession->sendData(cmd_connected, (void*)s.c_str(), s.size());
			}
			//delete[] s;
		}
	}
	else
	{
		pSession->destroy();
		SAFE_DELETE(pSession);
	}

	return pSession;
}

void CenterServerHandler::onSocketListenerError(void * userdata, int er) {
	LOGFMTD("got an error %d (%s) on the listener. shutting down.\n", er, evutil_socket_error_to_string(er));
	this->close();
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
