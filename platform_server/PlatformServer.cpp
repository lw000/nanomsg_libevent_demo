#include "PlatformServer.h"
	
#include <memory>

#include "socket_session.h"

#include "command.h"
#include "platform.pb.h"
#include "chat.pb.h"

#include <iostream>

#include "log4z.h"
#include "UserSession.h"
#include "socket_config.h"
#include "common_marco.h"

using namespace LW;

PlatformServerHandler::PlatformServerHandler()
{
}

PlatformServerHandler::~PlatformServerHandler()
{

}

SocketSession* PlatformServerHandler::onSocketListener(SocketProcessor* processor, evutil_socket_t fd)
{
	UserSession* pSession = nullptr;
	pSession = new UserSession;
	if (pSession != nullptr) {
		pSession->onDisconnectHandler = SOCKET_EVENT_SELECTOR(PlatformServerHandler::onSocketDisConnect, this);
		pSession->onTimeoutHandler = SOCKET_EVENT_SELECTOR(PlatformServerHandler::onSocketTimeout, this);
		pSession->onErrorHandler = SOCKET_EVENT_SELECTOR(PlatformServerHandler::onSocketError, this);
		pSession->onDataParseHandler = SOCKET_DATAPARSE_SELECTOR_4(PlatformServerHandler::onSocketParse, this);
		int r = pSession->create(processor, new SocketConfig, fd);
		if (r == 0)
		{
			int new_client_id = 0;
			{
				lw_lock_guard l(&_lock);
				new_client_id = _base_client_id++;
			}

			pSession->userinfo.uid = new_client_id;
			_users.add(pSession);

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
	}
	
	return pSession;
}

void PlatformServerHandler::onSocketDisConnect(SocketSession* session)
{
	_users.remove((UserSession*)session);
	
	LOGD("PlatformServerHandler::onSocketDisConnect");
}

void PlatformServerHandler::onSocketTimeout(SocketSession* session)
{
	_users.remove((UserSession*)session);

	LOGD("PlatformServerHandler::onSocketTimeout");
}

void PlatformServerHandler::onSocketError(SocketSession* session)
{
	_users.remove((UserSession*)session);

	LOGD("PlatformServerHandler::onSocketError");
}

int PlatformServerHandler::onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
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

	return 0;
}
