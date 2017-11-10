#ifndef __room_h__
#define __room_h__

#include <string>
#include <vector>
#include <unordered_map>

#include "common_struct.h"

#include "common_type.h"
#include "common_struct.h"
#include "GameDeskHandler.h"
#include "UserServer.h"
#include "lock.h"

class GameServerMgr;

class RoomServerMgr
{
	std::vector<RoomInfo> rooms;

private:
	UserServer _userserver;

public:
	RoomServerMgr();
	virtual ~RoomServerMgr();

public:
	bool create(const std::vector<RoomInfo>& infos);
	void destroy();

private:
	lw_fast_mutex _lock;
	std::unordered_map<int, GameServerMgr*> _game_mgr;
};

#endif
