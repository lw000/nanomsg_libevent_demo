#include "game_ai.h"

#include <iostream>

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

using namespace LW;


static const int GAME_AI_COUNT = 2000;

//////////////////////////////////////////////////////////////////////////////////////////
class GameAIHandler {
public:
	int ai_id;
	GameAI* _ai;
	SocketTimer *_timer;

public:
	SocketSession* _session;
	lw_fast_lock _lock;

public:
	GameAIHandler(GameAI* ai, int ai_id) {
		this->ai_id = ai_id;
		this->_ai = ai;
		this->_timer = new SocketTimer;

		this->_session = new SocketSession(new SocketConfig("127.0.0.1", 19801));
		this->_session->connectedHandler = SOCKET_EVENT_SELECTOR(GameAIHandler::onSocketConnected, this);
		this->_session->disConnectHandler = SOCKET_EVENT_SELECTOR(GameAIHandler::onSocketDisConnect, this);
		this->_session->timeoutHandler = SOCKET_EVENT_SELECTOR(GameAIHandler::onSocketTimeout, this);
		this->_session->errorHandler = SOCKET_EVENT_SELECTOR(GameAIHandler::onSocketError, this);
		this->_session->parseHandler = SOCKET_PARSE_SELECTOR_4(GameAIHandler::onSocketParse, this);
	}

	virtual ~GameAIHandler() {
		delete this->_timer;
	}

public:
	bool create(GameAIMgr* aimgr) {
		do
		{
			{
				int c = this->_timer->create(aimgr->_processor);
			}
			
			{
				int c = this->_session->create(SESSION_TYPE::client, aimgr->_processor);
				if (c == 0) {
					return true;
				}
				else {
					delete this->_session;
					this->_session = nullptr;
				}
			}
		} while (0);
		
		return false;
	}

public:
	int onSocketConnected(SocketSession* session)
	{
		this->_session = session;

		this->_timer->add(this->ai_id, 15000, [this](int tid, unsigned int tms) -> bool {
			{
				lw_lock_guard l(&_lock);
				if (this->_session->connected()) {
					{
						platform::msg_heartbeat msg;
						msg.set_time(time(NULL));
						int c = msg.ByteSizeLong();
						std::unique_ptr<char[]> s(new char[c]());
						bool ret = msg.SerializeToArray(s.get(), c);
						if (ret)
						{
							SocketRecvHandlerConf conf(cmd_heart_beat, 1, [](lw_char8* buf, lw_int32 bufsize) -> bool {
								platform::msg_heartbeat msg;
								msg.ParseFromArray(buf, bufsize);
								LOGFMTD("heartBeat[%d]", msg.time());

								return false;
							});

							this->_session->sendData(cmd_heart_beat, s.get(), c, conf);
						}
					}
				}
			}
			return true;
		});
		return 0;
	}

	int onSocketDisConnect(SocketSession* session)
	{
		return 0;
	}

	int onSocketTimeout(SocketSession* session)
	{
		return 0;
	}

	int onSocketError(SocketSession* session)
	{
		{
			lw_lock_guard l(&_lock);
			this->_ai->removeClient(this);
		}

		return 0;
	}

public:
	void onSocketParse(SocketSession* session, lw_int32 cmd,
		lw_char8* buf, lw_int32 bufsize)
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
		lw_lock_guard l(&_cli_lock);
		this->_clis.push_back(cli);
	}
}

void GameAI::removeClient(GameAIHandler* cli) {
	{
		lw_lock_guard l(&_cli_lock);
		std::vector<GameAIHandler*>::iterator iter = std::find_if(this->_clis.begin(), this->_clis.end(), [cli](GameAIHandler* c) -> bool {
			return (c == cli);
		});

		if (iter != this->_clis.end()) {
			delete *iter;

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

		Sleep(10);
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
	this->_timer = new SocketTimer;
}

GameAIMgr::~GameAIMgr() {
	delete this->_ai;
	delete this->_timer;
	delete this->_processor;
}

int GameAIMgr::onStart() {
	bool ret = this->_processor->create(false);
	if (ret) {
		this->_timer->create(this->_processor);
		this->_timer->add(0, 10000,
			[this](int tid, unsigned int tms) -> bool {
			//printf("tid: %d, tms: %d \n", tid, tms);
			return true;
		});

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