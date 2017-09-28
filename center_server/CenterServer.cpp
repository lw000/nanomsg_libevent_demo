#include "CenterServer.h"

#include "socket_session.h"

#include "command.h"
#include "platform.pb.h"

#include <iostream>

using namespace LW;

CenterServerHandler::CenterServerHandler()
{

}

CenterServerHandler::~CenterServerHandler()
{
	
}

void CenterServerHandler::sendHeartbeat(SocketSession* session)
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
}

void CenterServerHandler::onListener(SocketSession* session)
{
	Sessions.add(session);

	platform::msg_connected msg;
	lw_llong64 t = time(NULL);
	msg.set_time(t);
	int len = msg.ByteSize();
	{
		char *s = new char[len + 1];

		bool ret = msg.SerializeToArray(s, len);
		if (ret)
		{
			session->sendData(cmd_connected, s, len);
		}
		delete s;
	}
	std::cout << *session << std::endl;
}

int CenterServerHandler::onSocketConnected(SocketSession* session)
{
	return 0;
}

int CenterServerHandler::onSocketDisConnect(SocketSession* session)
{
	Sessions.remove(session);
	return 0;
}

int CenterServerHandler::onSocketTimeout(SocketSession* session)
{
	Sessions.remove(session);
	return 0;
}

int CenterServerHandler::onSocketError(SocketSession* session)
{
	Sessions.remove(session);

	return 0;
}

void CenterServerHandler::onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
{
	switch (cmd)
	{
	case cmd_heart_beat:
	{
		sendHeartbeat(session);	
	} break;
	default:
		break;
	}
}
