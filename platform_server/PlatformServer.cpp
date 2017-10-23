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

void PlatformServerHandler::loadConfig() {
	RoomInfo room_info;
	room_info.rid = 0;	// 房间编号
	room_info.deskcount = 1;// 桌子个数
	room_info.max_usercount = 10000;// 最大用户个数
	room_info.name = std::string("测试01房");	// 房间名称
	this->_roomserver.create(room_info);
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
				lw_fast_lock_guard l(&_lock);
				new_client_id = this->_base_client_id++;
			}

			pSession->userinfo.uid = new_client_id;
			this->_userserver.add(pSession);

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
	this->_userserver.remove((UserSession*)session);
}

void PlatformServerHandler::onSocketTimeout(SocketSession* session)
{
	this->_userserver.remove((UserSession*)session);
}

void PlatformServerHandler::onSocketError(SocketSession* session)
{
	this->_userserver.remove((UserSession*)session);
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
