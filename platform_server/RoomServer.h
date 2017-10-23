#ifndef __room_h__
#define __room_h__

#include <string>
#include <vector>

#include "common_struct.h"

#include "common_type.h"
#include "common_struct.h"
#include "GameHandler.h"
#include "UserServer.h"
#include "lock.h"

class GameServer;

class RoomServer
{
private:
	UserServer _userserver;

public:
	RoomServer();
	virtual ~RoomServer();

public:
	bool create(const RoomInfo& info);
	void destroy();

public:
	void addGameHandler(GameServer* desk);
	void removeGameHandler(GameServer* desk);

private:
	RoomInfo _room_info;

private:
	lw_fast_mutex _lock;
	std::vector<GameServer*> iGames;
};

#endif
