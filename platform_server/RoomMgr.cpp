#include <GameMgr.h>
#include <RoomMgr.h>
#include <algorithm>

#include "socket_config.h"
#include "socket_session.h"
#include "common_marco.h"
#include "UserSession.h"

#include "command.h"
#include "platform.pb.h"

using namespace LW;

RoomMgr::RoomMgr() {

}

RoomMgr::~RoomMgr() {

}

bool RoomMgr::create(const std::vector<RoomInfo>& infos) {
	this->rooms = infos;
	{
		lw_fast_lock_guard l(_lock);
		std::vector<RoomInfo>::iterator iter = this->rooms.end();
		for (; iter != this->rooms.end(); ++iter) {
			GameMgr * pGameMgr = nullptr;
			auto g = this->_games.find(iter->game_type);
			if (g == this->_games.end()) {
				pGameMgr = new GameMgr();
				if (pGameMgr->create(*iter)) {
					auto pair_game = std::pair<int, GameMgr*>(iter->game_type,
							pGameMgr);
					this->_games.insert(pair_game);
				}
			}
			else {
				pGameMgr = g->second;
			}
		}
	}

	return true;
}

void RoomMgr::destroy() {
	lw_fast_lock_guard l(_lock);
	auto iter = this->_games.begin();
	for (; iter != this->_games.end(); ++iter) {
		GameMgr* pDesk = iter->second;
		delete pDesk;
	}

	std::unordered_map<int, GameMgr*>().swap(this->_games);
}

void RoomMgr::join(UserSession* pSession) {
	this->_userserver.add(pSession);
}

void RoomMgr::leave(UserSession* pSession) {
	this->_userserver.remove(pSession);
}

void RoomMgr::onSocketDisConnect(UserSession* session) {

}

void RoomMgr::onSocketTimeout(UserSession* session) {

}

void RoomMgr::onSocketError(UserSession* session) {

}

int RoomMgr::onSocketParse(UserSession* session, lw_int32 cmd, lw_char8* buf,
		lw_int32 bufsize) {

	switch (cmd) {
		case p_cs_login_request: {

			break;
		}
		case p_cs_logout_request: {

			break;
		}
		case p_cs_frame_join_room_request: {

			break;
		}
		case p_cs_frame_leave_room_request: {

			break;
		}
		default: {
			break;
		}
	}

	return 0;
}
