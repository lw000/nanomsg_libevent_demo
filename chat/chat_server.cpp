#include "chat_server.h"

#include "common_marco.h"

#include "socket_processor.h"
#include "socket_session.h"
#include "socket_client.h"
#include "socket_config.h"

#include "command.h"
#include "platform.pb.h"
#include "chat.pb.h"


using namespace LW;

////////////////////////////////////////////////////////////////////////////////////////////////////

ChatServerHandler::ChatServerHandler()
{
	this->_client_id = 0;
}

ChatServerHandler::~ChatServerHandler()
{

}

bool ChatServerHandler::create(SocketConfig* conf)
{
	return true;
}

void ChatServerHandler::destroy()
{
}

void ChatServerHandler::onSocketListener(SocketSession* session) {
	
	int new_client_id = 0;
	{
		lw_lock_guard l(&_lock);
		new_client_id = _client_id++;
	}

	UserInfo user;
	user.uid = new_client_id;
	_users.add(user, session);

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
}

void ChatServerHandler::onSocketDisConnect(SocketSession* session)
{

}

void ChatServerHandler::onSocketTimeout(SocketSession* session)
{

}

void ChatServerHandler::onSocketError(SocketSession* session)
{

}

void ChatServerHandler::onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
{
	switch (cmd)
	{
	case cs_chat_request: {
		chat::msg_chat_request recv_msg;
		bool r = recv_msg.ParseFromArray(buf, bufsize);
		if (r) {
			{

			}

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
	case sc_chat_broadcast: {
		
		break;
	}

	default: {

		break;
	}
	}
}
