// GameServer.cpp
//

#include "main.h"

#if defined(WIN32) || defined(_WIN32)
#include <winsock2.h>
#endif // WIN32

#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <thread>

#include "command.h"
#include "platform.pb.h"
#include "socket_timer.h"
#include "lock.h"
#include "utils.h"

#include "client_http.h"

#include "GameServer.h"

#include "net.h"
#include "socket_processor.h"
#include "socket_session.h"
#include "Threadable.h"

using namespace LW;

class GameAITestTimer;
class GameTestClientHandler;

lw_fast_lock __g_lock;

static bool is_init = false;

//GameServer* __g_server[1024] = {0};

SocketClient* __g_game_clients[1024] = { 0 };

#define RPC_TIMES 1

class GameAITestTimer : public Threadable
{
	SocketProcessor _processor;
	Timer _timer;

public:
	GameAITestTimer() {

	}

	~GameAITestTimer() {

	}

protected:
	virtual int onStart() {
		_processor.create(false);
		_timer.create(&_processor);
		_timer.start(100, 100, [](int tid, unsigned int tms) -> bool
		{
			for (int i = 0; i < RPC_TIMES; i++) {
				if (__g_game_clients[i] != NULL) {
					if (__g_game_clients[i]->getSession()->connected()) {
						platform::msg_chat_request msg;
						msg.set_device(1);
						msg.set_from_uid(i);
						msg.set_to_uid(10000);
						msg.set_msg("{\"name\":\"liwei\",\"age\":30,\"sex\":1,\"address\":\"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\"}");
						int c = msg.ByteSizeLong();
						std::unique_ptr<char[]> s(new char[c]());
						bool ret = msg.SerializeToArray(s.get(), c);
						if (ret)
						{
							__g_game_clients[i]->getSession()->sendData(cmd_platform_cs_chat_request, s.get(), c);
						}
					}
				}
			}

			return true;
		});
		return 0;
	}

	virtual int onRun() {
		_processor.dispatch();
		return 0;
	}

	virtual int onEnd() {
		return 0;
	}
};

GameAITestTimer __g_ai_timer;

class GameTestClientHandler : public AbstractSocketClientHandler
{
public:
	SocketSession* _session;

public:
	GameTestClientHandler()
	{
	}

	virtual ~GameTestClientHandler()
	{
	}

protected:
	virtual int onSocketConnected(SocketSession* session) override
	{
		if (!is_init) {
			__g_lock.lock();
			if (!is_init) {
				is_init = true;
				__g_ai_timer.start();
			}
			__g_lock.unlock();
		}
		
		this->_session = session;

		return 0;
	}

	virtual int onSocketDisConnect(SocketSession* session) override
	{
		return 0;
	}

	virtual int onSocketTimeout(SocketSession* session) override
	{
		return 0;
	}

	virtual int onSocketError(SocketSession* session) override
	{
		return 0;
	}

protected:
	virtual void onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize) override
	{
		switch (cmd)
		{
		case cmd_connected:
		{
			platform::msg_connected msg;
			msg.ParseFromArray(buf, bufsize);
			printf("connected. [time : %d]\n", msg.time());

		} break;
		case cmd_heart_beat:
		{
			platform::msg_heartbeat msg;
			msg.ParseFromArray(buf, bufsize);
			printf("heartBeat[%d]\n", msg.time());
		} break;
		case cmd_platform_sc_userinfo:
		{
			platform::msg_userinfo_reponse msg;
			msg.ParseFromArray(buf, bufsize);
			printf("userid: %d name:%s \n", msg.uid(), msg.name().c_str());
		} break;
		case cmd_platform_sc_chat_reponse:
		{
			platform::msg_chat_reponse msg;
			msg.ParseFromArray(buf, bufsize);
			printf("from_uid: %d to_uid:%d\n", msg.from_uid(), msg.to_uid());
		} break;
		default:
			break;
		}
	}
};

// void signal_handler(int sig)
// {
// 	switch (sig)
// 	{
// 	case SIGTERM:
// 	case SIGINT:
// 	//case SIGBREAK:
// 	case SIGABRT:	
// 		//event_base_loopbreak(__http_base);
// 		break;
// 	}
// }

int game_server_main(int argc, char** argv)
{
// 	signal(SIGINT, signal_handler);
// 	signal(SIGABRT, signal_handler);
// 	signal(SIGTERM, signal_handler);

	SocketInit s;

	{
		for (int i = 0; i < RPC_TIMES; i++) {
		 	SocketClient *cli = new SocketClient;
			cli->connectedHandler = [](SocketSession* session) -> int {

				return 0;
			};

			cli->disConnectHandler = [](SocketSession* session) -> int {
				return 0;
			};

			cli->timeoutHandler = [](SocketSession* session) -> int  {
				return 0;
			};

			cli->errorHandler = [](SocketSession* session) -> int  {

				return 0;
			};

			bool r = cli->create(new GameTestClientHandler());
			if (r) {
				cli->run("127.0.0.1", 19801);
				__g_game_clients[i] = cli;
			}
			else {
				delete cli;
			}
		}
	}
// 	lw_int32 port;
// 	lw_int32 rpc_times;
// 	lw_int32 http_times;
// 
// 	port = 19801;
// 	rpc_times = 10;
// 	http_times = 1;
// 	 
// 	for (lw_int32 i = 0; i < rpc_times; i++) {
// 		DESK_INFO desk_info;
// 		desk_info.did = i;
// 		desk_info.max_usercount = 6;
// 		char buf[64];
// 		sprintf(buf, "[%d]", i);
// 		desk_info.name = std::string(buf);
// 		desk_info.rid = 0;
// 		desk_info.state = DESK_STATE_Empty;
// 
// 		GameServer * serv = new GameServer(nullptr);
// 		serv->connectedHandler = [&ai](SocketSession* session) -> int {
// 		
// 			if (!is_init) {
// 				__g_lock.lock();
// 				if (!is_init) {
// 					is_init = true;
// 					ai.start();
// 				}
// 				__g_lock.unlock();
// 			}
// 
// 			return 0;
// 		};
// 
// 		serv->disConnectHandler = [](SocketSession* session) -> int {
// 			return 0;
// 		};
// 
// 		serv->timeoutHandler = [](SocketSession* session) -> int  {
// 			return 0;
// 		};
// 
// 		serv->errorHandler = [](SocketSession* session) -> int  {
// 			return 0;
// 		};
// 
// 		if (serv->create(desk_info))
// 		{
// 			serv->start("127.0.0.1", port);
// 		}
// 
// 		__g_server[i] = serv;
// 
// 		lw_sleep(0.1);
// 	}
// 
// 	run_client_http(http_times);

	while (1)
	{
		lw_sleep(1);
	}

	return 0;
}

