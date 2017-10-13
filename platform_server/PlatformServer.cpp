#include "PlatformServer.h"
	
#include "socket_session.h"

#include "command.h"
#include "platform.pb.h"

#include <memory>

#include <iostream>

#include "log4z.h"

using namespace LW;

PlatformServerHandler::PlatformServerHandler()
{
}

PlatformServerHandler::~PlatformServerHandler()
{

}

AbstractUser* PlatformServerHandler::getUsers()
{
	return &this->users;
}

void PlatformServerHandler::onListener(SocketSession* session)
{
//	static int i = 0;
//	USER_INFO user;
//	user.uid = i++;
//	users.add(user, session);
//
//	const UserSession* us = users.find(user.uid);

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

	LOGD(session->debug());
}

int PlatformServerHandler::onSocketConnected(SocketSession* session)
{
	return 0;
}

int PlatformServerHandler::onSocketDisConnect(SocketSession* session)
{
	users.remove(session);
	return 0;
}

int PlatformServerHandler::onSocketTimeout(SocketSession* session)
{
	users.remove(session);
	return 0;
}

int PlatformServerHandler::onSocketError(SocketSession* session)
{
	users.remove(session);
	return 0;
}

void PlatformServerHandler::onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
{
	switch (cmd)
	{
	case cmd_connected:
	{
	} break;
	case cmd_heart_beat:
	{
		platform::msg_heartbeat msg;
		lw_llong64 t = time(NULL);
		msg.set_time(t);
		int c = msg.ByteSize();
		{
			std::unique_ptr<char[]> s(new char[c + 1]);
			bool ret = msg.SerializeToArray(s.get(), c);
			if (ret)
			{
				session->sendData(cmd_heart_beat, s.get(), c);
			}
		}

		break;
	}
	case cmd_platform_cs_chat_request:
	{
		platform::msg_chat_reponse recv_msg;
		bool r = recv_msg.ParseFromArray(buf, bufsize);
		if (r) {
			platform::msg_chat_reponse msg_reply;
			msg_reply.set_from_uid(recv_msg.from_uid());
			msg_reply.set_to_uid(recv_msg.to_uid());
			msg_reply.set_msg(recv_msg.msg());
			int c = msg_reply.ByteSizeLong();
			std::unique_ptr<char[]> s(new char[c]());
			bool ret = msg_reply.SerializeToArray(s.get(), c);
			if (ret)
			{
				session->sendData(cmd_platform_sc_chat_reponse, s.get(), c);
			}
		}	
		break;
	}
	default: {
		break;
	}	
	}
}
