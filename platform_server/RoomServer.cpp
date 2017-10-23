#include "RoomServer.h"

#include <algorithm>

#include "GameHandler.h"
#include "GameServer.h"
#include "socket_config.h"
#include "common_marco.h"

RoomServer::RoomServer()
{

}

RoomServer::~RoomServer()
{

}

bool RoomServer::create(const RoomInfo& info)
{
	this->_room_info = info;

	for (int i = 0; i < _room_info.deskcount; i++)
	{
		DeskInfo desk_info;
		desk_info.did = i;
		desk_info.max_usercount = 2;
		char buf[64];
		sprintf(buf, "²âÊÔ[%d]", i);
		desk_info.name = std::string(buf);
		desk_info.rid = this->_room_info.rid;
		desk_info.state = DESK_STATE_Empty;
		GameServer * pDesk = new GameServer(new GameHandler(desk_info));
		if (pDesk->create())
		{
			this->iGames.push_back(pDesk);
		}
	}

	return true;
}

void RoomServer::destroy()
{
	{
		lw_fast_lock_guard l(&_lock);
		std::vector<GameServer*>::iterator iter = this->iGames.begin();
		for (; iter != this->iGames.end(); ++iter)
		{
			GameServer* pDesk = *iter;
			delete pDesk;
		}
		std::vector<GameServer*>().swap(this->iGames);
	}
}

void RoomServer::addGameHandler(GameServer* desk) {
	{
		lw_fast_lock_guard l(&_lock);
		std::vector<GameServer*>::iterator iter = this->iGames.end();
		iter = std::find_if(this->iGames.begin(), this->iGames.end(), [desk](GameServer* c) -> bool {
			return (c == desk);
		});

		if (iter == this->iGames.end()) {
			this->iGames.push_back(desk);
		}
		else {
			*iter = desk;
		}
	}
}

void RoomServer::removeGameHandler(GameServer* desk) {
	{
		lw_fast_lock_guard l(&_lock);

		std::vector<GameServer*>::iterator iter = this->iGames.end();
		iter = std::find_if(this->iGames.begin(), this->iGames.end(), [desk](GameServer* c) -> bool {
			return (c == desk);
		});

		if (iter != this->iGames.end()) {
			GameServer* p = *iter;
			this->iGames.erase(iter);
			SAFE_DELETE(p);
		}
	}
}