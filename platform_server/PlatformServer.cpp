#include "PlatformServer.h"
	
#include <memory>

#include "socket_session.h"

#include "command.h"
#include "platform.pb.h"
#include "chat.pb.h"

#include <iostream>

#include "log4z.h"

using namespace LW;

static int __g_client_id_base = 0;

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

int PlatformServerHandler::onListener(SocketSession* session)
{
	int new_client_id = 0;
	
	{
		lw_lock_guard l(&_lock);
		new_client_id = __g_client_id_base++;
	}

	USER_INFO user;
	user.uid = new_client_id;
	users.add(user, session);

	platform::msg_connected msg;
	lw_llong64 t = time(NULL);
	msg.set_time(t);
	msg.set_client_id(new_client_id);
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

	return 0;
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
	case cs_chat_request:
	{
		chat::msg_chat_request recv_msg;
		bool r = recv_msg.ParseFromArray(buf, bufsize);
		if (r) {
			chat::msg_chat_reply msg_reply;
			msg_reply.set_from_uid(recv_msg.from_uid());
			msg_reply.set_to_uid(recv_msg.to_uid());
			msg_reply.set_msg(recv_msg.msg());
			int c = msg_reply.ByteSizeLong();
			std::unique_ptr<char[]> s(new char[c]());
			bool ret = msg_reply.SerializeToArray(s.get(), c);
			if (ret)
			{
				session->sendData(sc_chat_reply, s.get(), c);
			}
		}	
		break;
	}
	default: {
		break;
	}	
	}
}
