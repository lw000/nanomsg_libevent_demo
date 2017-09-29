#include "client_main.h"

#include <stdio.h>
#include <iostream>
#include <signal.h>

#include "command.h"
#include "platform.pb.h"

#include "socket_processor.h"
#include "socket_client.h"
#include "socket_session.h"
#include "socket_timer.h"

using namespace LW;

class AITestTimer;
class PlatformClientHandler;

static SocketClient __g_platform_client;

class AITestTimer : public Threadable
{
	SocketProcessor _processor;
	Timer _timer;

public:
	AITestTimer() {

	}

	~AITestTimer() {

	}

protected:
	virtual int onStart() {
		_processor.create(false);
		_timer.create(&_processor);
		_timer.start(100, 15000, [](int tid, unsigned int tms) -> bool
		{
			platform::msg_heartbeat msg;
			msg.set_time(time(NULL));
			lw_int32 c = (lw_int32)msg.ByteSizeLong();
			std::unique_ptr<char[]> s(new char[c]());
			lw_bool ret = msg.SerializeToArray(s.get(), c);
			if (ret)
			{
				__g_platform_client.getSession()->sendData(cmd_heart_beat, s.get(), c);
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


class PlatformClientHandler : public AbstractSocketClientHandler
{
	AITestTimer __g_timer_test;

public:
	PlatformClientHandler()
	{
	}

	virtual ~PlatformClientHandler()
	{
	}

protected:
	virtual int onSocketConnected(SocketSession* session) override
	{
		__g_timer_test.start();
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
			printf("userid: %d age:%d sex:%d name:%s address:%s\n", msg.uid(),
				msg.age(), msg.sex(), msg.name().c_str(), msg.address().c_str());
		} break;
		default:
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////////////////////////////

int __connect_center_server(const lw_char8* addr, lw_short16 port)
{
	if (__g_platform_client.create(new PlatformClientHandler()))
	{
		int ret = __g_platform_client.run(addr, port);
	}
	return 0;
}

