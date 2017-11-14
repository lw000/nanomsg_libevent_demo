#include "RoomServerMgr.h"

#include <algorithm>

#include "socket_config.h"
#include "common_marco.h"
#include "GameServerMgr.h"

#include "UserSession.h"

RoomServerMgr::RoomServerMgr()
{

}

RoomServerMgr::~RoomServerMgr()
{

}

bool RoomServerMgr::create(const std::vector<RoomInfo>& infos) {
	this->rooms = infos;
	{
		lw_fast_lock_guard l(_lock);
		for (auto r : this->rooms) {
			GameServerMgr * pDeskMgr = nullptr;
			auto g = this->_games.find(r.game_type);
			if (g == this->_games.end()) {
				pDeskMgr = new GameServerMgr();
				if (pDeskMgr->create(r))
				{
					this->_games.insert(std::pair<int, GameServerMgr*>(r.game_type, pDeskMgr));
				}
			}
			else {
				pDeskMgr = g->second;
			}
		}
	}

	return true;
}

void RoomServerMgr::destroy()
{
	{
		lw_fast_lock_guard l(_lock);
		auto iter = this->_games.begin();
		for (; iter != this->_games.end(); ++iter)
		{
			GameServerMgr* pDesk = iter->second;
			delete pDesk;
		}

		std::unordered_map<int, GameServerMgr*>().swap(this->_games);
	}
}

void RoomServerMgr::join(UserSession* pSession) {
	this->_userserver.add(pSession);
}

void RoomServerMgr::leave(UserSession* pSession) {
	this->_userserver.remove(pSession);
}
