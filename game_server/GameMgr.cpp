#include <GameMgr.h>
#include "common_marco.h"

#include "socket_processor.h"
#include "socket_session.h"
#include "client_session.h"
#include "socket_client.h"

#include "command.h"
#include "platform.pb.h"
#include "socket_config.h"

#include "GameDeskHandler.h"

using namespace LW;

////////////////////////////////////////////////////////////////////////////////////////////////////

GameMgr::GameMgr()
{
	
}

GameMgr::~GameMgr()
{
	this->destroy();

}

bool GameMgr::create(const RoomInfo& r)
{
	for (int i = 0; i < r.deskcount; i++)
	{
		DeskInfo desk;
		desk.did = i;
		desk.max_usercount = 2;
		char buf[64];
		sprintf(buf, "game[%d]", i);
		desk.name = std::string(buf);
		desk.rid = r.rid;
		desk.state = DESK_STATE_Empty;
		GameDeskHandler * handler = new GameDeskHandler(desk);
		_games.push_back(handler);
	}

	return true;
}

void GameMgr::destroy()
{

}

void GameMgr::sendData(lw_int32 cmd, void* object, lw_int32 objectSize)
{
	
}

lw_int32 GameMgr::sendData(lw_int32 cmd, void* object, lw_int32 objectSize, const SendDataCallback& cb) {
	
	return 0;
}

void GameMgr::onSocketConnected(SocketSession* session)
{

}

void GameMgr::onSocketDisConnect(SocketSession* session)
{

}

void GameMgr::onSocketTimeout(SocketSession* session)
{

}

void GameMgr::onSocketError(SocketSession* session)
{

}

int GameMgr::onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
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

int GameMgr::frameMessage(int cmd, void* data, int datasize)
{
// 	switch (cmd)
// 	{
// 	case g_frame_cs_game_start:
// 	{
// 		this->_gameHandler->onGameStartReponse(data, datasize);
// 	} break;
// 	case g_frame_sc_game_end:
// 	{
// 		this->_gameHandler->onGameEndReponse(data, datasize);
// 	} break;
// 	case g_frame_sc_sit_up:
// 	{
// 		this->_gameHandler->onGameUserSitupReponse(data, datasize);
// 	} break;
// 	case g_frame_sc_sit_down:
// 	{
// 		this->_gameHandler->onGameUserSitdownReponse(data, datasize);
// 	} break;
// 	default:
// 		break;
// 	}

	return 0;
}

int GameMgr::onGameMessage(int cmd, void* data, int datasize)
{
	return 0;
}

void GameMgr::sendSitup(int uid)
{

}

void GameMgr::sendSitdown(int uid)
{

}
