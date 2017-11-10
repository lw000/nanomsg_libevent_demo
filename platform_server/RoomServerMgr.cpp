#include "RoomServerMgr.h"

#include <algorithm>

#include "socket_config.h"
#include "common_marco.h"
#include "GameServerMgr.h"

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
			auto g = this->_game_mgr.find(r.game_type);
			if (g == this->_game_mgr.end()) {
				pDeskMgr = new GameServerMgr();
				if (pDeskMgr->create(r))
				{
					this->_game_mgr.insert(std::pair<int, GameServerMgr*>(r.game_type, pDeskMgr));
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
		auto iter = this->_game_mgr.begin();
		for (; iter != this->_game_mgr.end(); ++iter)
		{
			GameServerMgr* pDesk = iter->second;
			delete pDesk;
		}

		std::unordered_map<int, GameServerMgr*>().swap(this->_game_mgr);
	}
}