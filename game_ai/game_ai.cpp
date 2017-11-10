#include "game_ai.h"

#include <iostream>
#include <algorithm>

#include "socket_processor.h"
#include "socket_session.h"
#include "socket_config.h"
#include "socket_timer.h"

#include "socket_hanlder.h"

#include "command.h"
#include "platform.pb.h"
#include "game.pb.h"
#include "chat.pb.h"

#include "log4z.h"
#include "utils.h"
#include "common_marco.h"

using namespace LW;


static const int GAME_AI_COUNT = 100;

//////////////////////////////////////////////////////////////////////////////////////////
class GameAIHandler {
public:
	int ai_id;
	GameAI* _ai;
	GameAIMgr* _aimgr;

public:
	SocketSession* _session;

public:
	GameAIHandler(GameAI* ai, int ai_id) {
		this->ai_id = ai_id;
		this->_ai = ai;	
		this->_aimgr = nullptr;
		this->_session = nullptr;
	}

	virtual ~GameAIHandler() {
		SAFE_DELETE(this->_session);
	}

public:
	bool create(GameAIMgr* aimgr) {
		do
		{
			this->_aimgr = aimgr;
			
			this->_session = new SocketSession();

			if (this->_session != nullptr) {
				this->_session->onConnectedHandler = SOCKET_EVENT_SELECTOR(GameAIHandler::onSocketConnected, this);
				this->_session->onDisconnectHandler = SOCKET_EVENT_SELECTOR(GameAIHandler::onSocketDisConnect, this);
				this->_session->onTimeoutHandler = SOCKET_EVENT_SELECTOR(GameAIHandler::onSocketTimeout, this);
				this->_session->onErrorHandler = SOCKET_EVENT_SELECTOR(GameAIHandler::onSocketError, this);
				this->_session->onDataParseHandler = SOCKET_DATAPARSE_SELECTOR_4(GameAIHandler::onSocketParse, this);
				int c = this->_session->create(SESSION_TYPE::client, this->_aimgr->_processor, new SocketConfig("127.0.0.1", 19801));
				if (c == 0) {
					_session->startAutoPing(5000);
					return true;
				}
				else {
					SAFE_DELETE(this->_session);
				}
			}
		} while (0);
		
		return false;
	}

public:
	void onSocketConnected(SocketSession* session)
	{
		this->_session = session;

		this->_aimgr->addTimer(this->ai_id, 15000, [this](int tid, unsigned int tms) -> bool {
			{
				if (this->_session->connected()) {
					{
						platform::msg_heartbeat msg;
						msg.set_time(time(NULL));
						int c = msg.ByteSizeLong();
						std::unique_ptr<char[]> s(new char[c]());
						bool ret = msg.SerializeToArray(s.get(), c);
						if (ret)
						{
							SendDataCallback cb(cmd_heart_beat, 1, [](lw_char8* buf, lw_int32 bufsize) -> bool {
								platform::msg_heartbeat msg;
								msg.ParseFromArray(buf, bufsize);
								LOGFMTD("heartBeat[%d]", msg.time());
								return false;
							});

							this->_session->sendData(cmd_heart_beat, s.get(), c, cb);
						}
					}
				}
			}
			return true;
		});
	}

	void onSocketDisConnect(SocketSession* session)
	{
		this->_aimgr->removeTimer(this->ai_id);
	}

	void onSocketTimeout(SocketSession* session)
	{
		this->_aimgr->removeTimer(this->ai_id);
	}

	void onSocketError(SocketSession* session)
	{
		this->_aimgr->removeTimer(this->ai_id);
		this->_ai->removeClient(this);
	}

public:
	int onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
	{
		switch (cmd) {
		case cmd_connected: {
			platform::msg_connected msg;
			msg.ParseFromArray(buf, bufsize);
			LOGFMTD("client_id[%d], connected. [time : %d]", msg.client_id(), msg.time());
			break;
		}
		case cmd_heart_beat: {
			platform::msg_heartbeat msg;
			msg.ParseFromArray(buf, bufsize);
			LOGFMTD("heartBeat[%d]", msg.time());

			break;
		}
		case sc_chat_reply: {
			chat::msg_chat_reply msg;
			msg.ParseFromArray(buf, bufsize);
			LOGFMTD("from_uid: %d to_uid:%d msg:%s", msg.from_uid(), msg.to_uid(), msg.msg().c_str());

			break;
		}
		default: {
			break;
		}
		}

		return 0;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////

GameAI::GameAI(GameAIMgr* aimgr){
	this->_aimgr = aimgr;
}

GameAI::~GameAI() {
}

void GameAI::addClient(GameAIHandler* cli) {
	{
		lw_fast_lock_guard l(_cli_lock);
		this->_clis.push_back(cli);
	}
}

void GameAI::removeClient(GameAIHandler* cli) {
	{
		lw_fast_lock_guard l(_cli_lock);
		std::vector<GameAIHandler*>::iterator iter = this->_clis.end();
		iter = std::find_if(this->_clis.begin(), this->_clis.end(), [cli](GameAIHandler* c) -> bool {
			return (c == cli);
		});

		if (iter != this->_clis.end()) {
			this->_clis.erase(iter);
		}
	}
}

int GameAI::onStart() {
	
	return 0;
}

int GameAI::onRun() {

	for (int i = 1000; i < GAME_AI_COUNT + 1000; i++) {
		GameAIHandler* gai = new GameAIHandler(this, i);
		bool r = gai->create(this->_aimgr);
		if (r) {
			this->addClient(gai);
		}
		else {
			delete gai;
		}

		lw_sleep(10);
	}

	return 0;
}

int GameAI::onEnd() {

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
GameAIMgr::GameAIMgr() {
	this->_processor = new SocketProcessor;
	this->_ai = new GameAI(this);
}

GameAIMgr::~GameAIMgr() {
	delete this->_ai;
	delete this->_processor;
}

int GameAIMgr::addTimer(int tid, unsigned int tms, const TimerCallback& func) {
	int c = this->_processor->addTimer(tid, tms, func);
	return c;
}

void GameAIMgr::removeTimer(int tid) {
	this->_processor->removeTimer(tid);
}

void GameAIMgr::addClient(GameAIHandler* cli) {
	{
		lw_fast_lock_guard l(_cliAiLock);
		this->_cliAis.push_back(cli);
	}
}

void GameAIMgr::removeClient(GameAIHandler* cli) {
	{
		lw_fast_lock_guard l(_cliAiLock);
		std::vector<GameAIHandler*>::iterator iter = this->_cliAis.end();
		iter = std::find_if(this->_cliAis.begin(), this->_cliAis.end(), [cli](GameAIHandler* c) -> bool {
			return (c == cli);
		});

		if (iter != this->_cliAis.end()) {
			this->_cliAis.erase(iter);
		}
	}
}

int GameAIMgr::onStart() {
	bool ret = this->_processor->create(false);
	if (ret) {
		this->_ai->start();
	}
	return 0;
}

int GameAIMgr::onRun() {
	int ret = this->_processor->dispatch();
	printf("ret = %d", ret);
	return 0;
}

int GameAIMgr::onEnd() {
	return 0;
}

GameAIMgr* __g_ai_mgr;

int main_ai_server(int argc, char** argv) {
	
	__g_ai_mgr = new GameAIMgr;
	__g_ai_mgr->start();

	while (1)
	{
		lw_sleep(10);
	}

	return 0;
}
