#include "GameClient.h"

#include "common_marco.h"

#include "socket_session.h"
#include "client_session.h"
#include "socket_client.h"

#include "command.h"
#include "platform.pb.h"

#include <iostream>
#include "socket_config.h"

using namespace LW;

////////////////////////////////////////////////////////////////////////////////////////////////////

GameClient::GameClient(AbstractGameClientHandler* handler) : _gameHandler(handler)
{
	_cli = new SocketClient;
	_cli->onConnectedHandler = SOCKET_EVENT_SELECTOR(GameClient::onSocketConnected, this);
	_cli->onDisconnectHandler = SOCKET_EVENT_SELECTOR(GameClient::onSocketDisConnect, this);
	_cli->onTimeoutHandler = SOCKET_EVENT_SELECTOR(GameClient::onSocketTimeout, this);
	_cli->onErrorHandler = SOCKET_EVENT_SELECTOR(GameClient::onSocketError, this);
	_cli->onDataParseHandler = SOCKET_DATAPARSE_SELECTOR_4(GameClient::onSocketParse, this);
}

GameClient::~GameClient()
{
	this->destroy();
	SAFE_DELETE(_cli);
}

bool GameClient::create(SocketConfig* conf)
{
	bool ret = false;
	do 
	{
		ret = _cli->create(conf);
		if (!ret) {
			break;
		}

		_cli->setAutoHeartBeat(5000);

		ret = true;

	} while (0);

	return ret;
}

void GameClient::destroy()
{
	_cli->destroy();
}

void GameClient::sendData(lw_int32 cmd, void* object, lw_int32 objectSize)
{
	_cli->getSession()->sendData(cmd, object, objectSize);
}

lw_int32 GameClient::sendData(lw_int32 cmd, void* object, lw_int32 objectSize, const SendDataCallback& cb) {
	int c = _cli->getSession()->sendData(cmd, object, objectSize, cb);
	return c;
}

void GameClient::onSocketConnected(SocketSession* session)
{

}

void GameClient::onSocketDisConnect(SocketSession* session)
{
	_cli->close();
}

void GameClient::onSocketTimeout(SocketSession* session)
{
	_cli->close();
}

void GameClient::onSocketError(SocketSession* session)
{
	_cli->close();
}

int GameClient::onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
{
	switch (cmd)
	{
	case cmd_connected:
	{
		printf("cmd_connected\n");
	} break;
	case g_frame_cs_game_start:
	case g_frame_sc_game_end:
	case g_frame_sc_sit_up:
	case g_frame_sc_sit_down:
	{
		this->frameMessage(cmd, buf, bufsize);
	} break;
	default:
	{
		this->onGameMessage(cmd, buf, bufsize);
	} break;
	}

	return 0;
}

int GameClient::frameMessage(int cmd, void* data, int datasize)
{
	switch (cmd)
	{
	case g_frame_cs_game_start:
	{
		this->_gameHandler->onGameStartReponse(data, datasize);
	} break;
	case g_frame_sc_game_end:
	{
		this->_gameHandler->onGameEndReponse(data, datasize);
	} break;
	case g_frame_sc_sit_up:
	{
		this->_gameHandler->onGameUserSitupReponse(data, datasize);
	} break;
	case g_frame_sc_sit_down:
	{
		this->_gameHandler->onGameUserSitdownReponse(data, datasize);
	} break;
	default:
		break;
	}

	return 0;
}

int GameClient::onGameMessage(int cmd, void* data, int datasize)
{
	int c = this->_gameHandler->onGameMessage(cmd, data, datasize);
	return c;
}

void GameClient::sendSitup(int uid)
{
	platform::msg_situp_request situp;
	situp.set_uid(uid);
	int c = situp.ByteSizeLong();
	std::unique_ptr<char[]> s(new char[c]);
	bool ret = situp.SerializeToArray(s.get(), c);
	_cli->getSession()->sendData(g_frame_cs_sit_up, s.get(), c);
}

void GameClient::sendSitdown(int uid)
{
	platform::msg_sitdown_request sitdown;
	sitdown.set_uid(uid);
	int c = sitdown.ByteSizeLong();
	std::unique_ptr<char[]> s(new char[c]);
	bool ret = sitdown.SerializeToArray(s.get(), c);
	_cli->getSession()->sendData(g_frame_cs_sit_down, s.get(), c);
}
