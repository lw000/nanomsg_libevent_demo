#include "GameDeskHandler.h"

#include <string>
#include <vector>

#include "command.h"
#include "game.pb.h"
#include "platform.pb.h"

#include "UserSession.h"

#include "log4z.h"

GameDeskHandler::GameDeskHandler(const DeskInfo& info) {
	this->_desk_info = info;
}

GameDeskHandler::~GameDeskHandler() {

}

void GameDeskHandler::onGameStartReponse(void* data, int datasize) {
	platform::msg_game_start_reponse gstart;
	gstart.ParseFromArray(data, datasize);
	LOGFMTD("game_msg_start_reponse: state: %d\n", gstart.state());
}

void GameDeskHandler::onGameEndReponse(void* data, int datasize) {
	platform::msg_game_end_reponse gend;
	gend.ParseFromArray(data, datasize);
	LOGFMTD("game_msg_end_reponse: state: %d\n", gend.state());
}

void GameDeskHandler::onGameUserSitupReponse(void* data, int datasize) {
	platform::msg_situp_reponse situp;
	situp.ParseFromArray(data, datasize);
	LOGFMTD("game_msg_situp_reponse uid: %d seat:%d lookup:%d\n", situp.uid(),
			situp.seat(), situp.lookup());
}

void GameDeskHandler::onGameUserSitdownReponse(void* data, int datasize) {
	platform::msg_sitdown_reponse sitdown;
	sitdown.ParseFromArray(data, datasize);
	LOGFMTD("game_msg_sitdown_reponse uid: %d seat:%d lookup: %d\n",
			sitdown.uid(), sitdown.seat(), sitdown.lookup());
}

int GameDeskHandler::onGameMessage(int cmd, void* data, int datasize) {
	switch (cmd) {
		default:
			break;
	}

	return 0;
}
