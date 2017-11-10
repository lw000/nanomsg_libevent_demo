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
	this->listenHandler = SOCKET_LISTENER_SELECTOR_2(PlatformServerHandler::onSocketListener, this);
	this->listenErrorHandler = SOCKET_LISTENER_SELECTOR_2(PlatformServerHandler::onSocketListenerError, this);
}

PlatformServerHandler::~PlatformServerHandler()
{

}

void PlatformServerHandler::loadConfig() {
	std::vector<RoomInfo> rooms;
	for (int i = 0; i < 1; i++) {
		RoomInfo room_info;
		room_info.rid = 0;	// 房间编号
		room_info.deskcount = 1;// 桌子个数
		room_info.max_usercount = 10000;// 最大用户个数
		room_info.name = std::string("测试01房");	// 房间名称
		rooms.push_back(room_info);
	}
	this->_roomserver.create(rooms);
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
				lw_fast_lock_guard l(_lock);
				new_client_id = this->_base_client_id++;
			}

			pSession->userinfo.uid = new_client_id;
			this->_userserver.add(pSession);

			platform::msg_connected msg;
			msg.set_time(time(NULL));
			int len = msg.ByteSize();
			{
				std::string s;
				bool ret = msg.SerializeToString(&s);
				if (ret)
				{
					pSession->sendData(cmd_connected, (void*)s.c_str(), s.size());
				}
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

void PlatformServerHandler::onSocketListenerError(void * userdata, int er) {
	LOGFMTD("got an error %d (%s) on the listener. shutting down.\n", er, evutil_socket_error_to_string(er));
	this->close();
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
	UserSession* pUserSession = (UserSession*)session;

	switch (cmd)
	{
	case p_cs_login_request: {

		break;
	}
	case p_cs_logout_request: {
		
		break;
	}
	case p_frame_join_room_request: {

		break;
	}
	case p_frame_leave_room_request: {

		break;
	}
	default: {
		break;
	}	
	}

	return 0;
}
