#ifndef __RoomMgr_H__
#define __RoomMgr_H__

#include <GameMgr.h>
#include <UserMgr.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "common_struct.h"

#include "common_type.h"
#include "common_struct.h"
#include "GameDeskHandler.h"
#include "lock.h"

class GameMgr;
class UserSession;

class RoomMgr {
	private:
		std::vector<RoomInfo> rooms;

	private:
		UserMgr _userserver;

	public:
		RoomMgr();
		virtual ~RoomMgr();

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
		int onSocketParse(UserSession* session, lw_int32 cmd, lw_char8* buf,
				lw_int32 bufsize);

	private:
		lw_fast_mutex _lock;
		std::unordered_map<int, GameMgr*> _games;
};

#endif	//__RoomMgr_H__
