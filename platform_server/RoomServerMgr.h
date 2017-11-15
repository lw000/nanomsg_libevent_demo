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
class UserSession;

class RoomServerMgr
{
private:
	std::vector<RoomInfo> rooms;

private:
	UserServer _userserver;

public:
	RoomServerMgr();
	virtual ~RoomServerMgr();

public:
	bool create(const std::vector<RoomInfo>& infos);
	void destroy();

public:
	void join(UserSession* pSession);
	void leave(UserSession* pSession);
	void find(UserSession* pSession);

public:
	void onSocketDisConnect(UserSession* session);
	void onSocketTimeout(UserSession* session);
	void onSocketError(UserSession* session);

public:
	int onSocketParse(UserSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize);

private:
	lw_fast_mutex _lock;
	std::unordered_map<int, GameServerMgr*> _games;
};

#endif