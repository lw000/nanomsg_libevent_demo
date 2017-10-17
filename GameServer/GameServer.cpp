#include "GameServer.h"

#include "common_marco.h"

#include "socket_processor.h"
#include "socket_session.h"
#include "socket_client.h"

#include "command.h"
#include "platform.pb.h"

#include <iostream>
#include "socket_config.h"

using namespace LW;

////////////////////////////////////////////////////////////////////////////////////////////////////

GameServer::GameServer(AbstractGameClientHandler* handler) : _handler(handler)
{
	_cli = new SocketClient;
	_cli->connectedHandler = SOCKET_EVENT_SELECTOR(GameServer::onSocketConnected, this);
	_cli->disConnectHandler = SOCKET_EVENT_SELECTOR(GameServer::onSocketDisConnect, this);
	_cli->timeoutHandler = SOCKET_EVENT_SELECTOR(GameServer::onSocketTimeout, this);
	_cli->errorHandler = SOCKET_EVENT_SELECTOR(GameServer::onSocketError, this);
	_cli->parseHandler = SOCKET_PARSE_SELECTOR_4(GameServer::onSocketParse, this);
}

GameServer::~GameServer()
{
	this->destroy();
	SAFE_DELETE(_cli);
}

bool GameServer::create(SocketConfig* conf)
{
	bool ret = false;
	do 
	{
		ret = _cli->create(conf);
		if (!ret) {
			break;
		}

		ret = true;

	} while (0);

	return ret;
}

void GameServer::destroy()
{
	_cli->destroy();
}

void GameServer::sendData(lw_int32 cmd, void* object, lw_int32 objectSize)
{
	_cli->getSession()->sendData(cmd, object, objectSize);
}

lw_int32 GameServer::sendData(lw_int32 cmd, void* object, lw_int32 objectSize, const SocketRecvHandlerConf& cb) {
	int c = _cli->getSession()->sendData(cmd, object, objectSize, cb);
	return c;
}

int GameServer::onSocketConnected(SocketSession* session)
{
	_cli->startTimer(100, 10000, [this](int tid, unsigned int tms) -> bool {
		platform::msg_heartbeat msg;
		msg.set_time(time(NULL));
		int c = msg.ByteSizeLong();
		std::unique_ptr<char[]> s(new char[c]());
		bool ret = msg.SerializeToArray(s.get(), c);
		if (ret)
		{
			this->_cli->getSession()->sendData(cmd_heart_beat, s.get(), c);
		}
		return true;
	});

	return 0;
}

int GameServer::onSocketDisConnect(SocketSession* session)
{
	_cli->close();

	return 0;
}

int GameServer::onSocketTimeout(SocketSession* session)
{
	_cli->close();

	return 0;
}

int GameServer::onSocketError(SocketSession* session)
{
	_cli->close();
	return 0;
}

void GameServer::onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
{
	switch (cmd)
	{
	case cmd_connected:
	{
		printf("cmd_connected\n");
	} break;
	case cmd_game_frame_cs_game_start:
	case cmd_game_frame_sc_game_end:
	case cmd_game_frame_sc_sit_up:
	case cmd_game_frame_sc_sit_down:
	{
		this->frameMessage(cmd, buf, bufsize);
	} break;
	default:
	{
		this->onGameMessage(cmd, buf, bufsize);
	} break;
	}
}

int GameServer::frameMessage(int cmd, void* data, int datasize)
{
	switch (cmd)
	{
	case cmd_game_frame_cs_game_start:
	{
		this->_handler->onGameStartReponse(data, datasize);
	} break;
	case cmd_game_frame_sc_game_end:
	{
		this->_handler->onGameEndReponse(data, datasize);
	} break;
	case cmd_game_frame_sc_sit_up:
	{
		this->_handler->onGameUserSitupReponse(data, datasize);
	} break;
	case cmd_game_frame_sc_sit_down:
	{
		this->_handler->onGameUserSitdownReponse(data, datasize);
	} break;
	default:
		break;
	}

	return 0;
}

int GameServer::onGameMessage(int cmd, void* data, int datasize)
{
	int c = this->_handler->onGameMessage(cmd, data, datasize);
	return c;
}

void GameServer::sendSitup(int uid)
{
	platform::msg_situp_request situp;
	situp.set_uid(uid);
	int c = situp.ByteSizeLong();
	std::unique_ptr<char[]> s(new char[c + 1]);
	bool ret = situp.SerializeToArray(s.get(), c);
	_cli->getSession()->sendData(cmd_game_frame_cs_sit_up, s.get(), c);
}

void GameServer::sendSitdown(int uid)
{
	platform::msg_sitdown_request sitdown;
	sitdown.set_uid(uid);
	int c = sitdown.ByteSizeLong();
	std::unique_ptr<char[]> s(new char[c + 1]);
	bool ret = sitdown.SerializeToArray(s.get(), c);
	_cli->getSession()->sendData(cmd_game_frame_cs_sit_down, s.get(), c);
}
