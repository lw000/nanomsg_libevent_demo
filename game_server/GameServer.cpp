#include "GameServer.h"

#include "common_marco.h"

#include "socket_processor.h"
#include "socket_session.h"
#include "client_session.h"
#include "socket_client.h"

#include "command.h"
#include "platform.pb.h"

#include <iostream>
#include "socket_config.h"

using namespace LW;

////////////////////////////////////////////////////////////////////////////////////////////////////

GameServer::GameServer(AbstractGameServerHandler* handler) : _gameHandler(handler)
{
}

GameServer::~GameServer()
{
	this->destroy();

}

bool GameServer::create()
{
	return 0;
}

void GameServer::destroy()
{

}

void GameServer::sendData(lw_int32 cmd, void* object, lw_int32 objectSize)
{
	
}

lw_int32 GameServer::sendData(lw_int32 cmd, void* object, lw_int32 objectSize, const SendDataCallback& cb) {
	
	return 0;
}

void GameServer::onSocketConnected(SocketSession* session)
{

}

void GameServer::onSocketDisConnect(SocketSession* session)
{

}

void GameServer::onSocketTimeout(SocketSession* session)
{

}

void GameServer::onSocketError(SocketSession* session)
{

}

int GameServer::onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
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

int GameServer::frameMessage(int cmd, void* data, int datasize)
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

int GameServer::onGameMessage(int cmd, void* data, int datasize)
{
	int c = this->_gameHandler->onGameMessage(cmd, data, datasize);
	return c;
}

void GameServer::sendSitup(int uid)
{

}

void GameServer::sendSitdown(int uid)
{

}
