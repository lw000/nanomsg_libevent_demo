#include "CenterServer.h"

#include "command.h"
#include "platform.pb.h"

#include "socket_session.h"

#include "log4z.h"

using namespace LW;

CenterServerHandler::CenterServerHandler()
{

}

CenterServerHandler::~CenterServerHandler()
{
	
}

void CenterServerHandler::onSocketListener(SocketSession* session)
{
	Sessions.add(session);

	platform::msg_connected msg;
	msg.set_time(time(NULL));
	int len = msg.ByteSize();
	{
		char *s = new char[len];
		bool ret = msg.SerializeToArray(s, len);
		if (ret)
		{
			session->sendData(cmd_connected, s, len);
		}
		delete[] s;
	}

	LOGFMTD("%s\n", session->debug().c_str());

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

void CenterServerHandler::onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
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
}
