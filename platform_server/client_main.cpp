#include "client_main.h"

#include <stdio.h>
#include <signal.h>

#include "common_marco.h"

#include "command.h"
#include "platform.pb.h"

#include "socket_processor.h"
#include "socket_client.h"
#include "socket_session.h"
#include "socket_timer.h"
#include "socket_config.h"

using namespace LW;

class PlatformClientHandler;

class PlatformClientHandler
{
	SocketSession* _session;
	SocketClient* _client;

public:
	PlatformClientHandler() : _session(nullptr)
	{
		this->_client = new SocketClient;
		this->_client->connectedHandler = SOCKET_EVENT_SELECTOR(PlatformClientHandler::onSocketConnected, this);
		this->_client->disConnectHandler = SOCKET_EVENT_SELECTOR(PlatformClientHandler::onSocketDisConnect, this);
		this->_client->timeoutHandler = SOCKET_EVENT_SELECTOR(PlatformClientHandler::onSocketTimeout, this);
		this->_client->errorHandler = SOCKET_EVENT_SELECTOR(PlatformClientHandler::onSocketError, this);
		this->_client->parseHandler = SOCKET_PARSE_SELECTOR_4(PlatformClientHandler::onSocketParse, this);
	}

	virtual ~PlatformClientHandler()
	{
		SAFE_DELETE(this->_client);
	}

public:
	bool create(const lw_char8* addr, lw_short16 port) {
	
		if (this->_client->create(new SocketConfig(addr, port)))
		{

		}

		_client->addTimer(100, 15000, [this](int tid, unsigned int tms) -> bool
		{
			platform::msg_heartbeat msg;
			msg.set_time(time(NULL));
			lw_int32 c = (lw_int32)msg.ByteSizeLong();
			std::unique_ptr<char[]> s(new char[c]());
			lw_bool ret = msg.SerializeToArray(s.get(), c);
			if (ret)
			{
				_session->sendData(cmd_heart_beat, s.get(), c);
			}
			return true;
		});

		return true;
	}

public:
	void onSocketConnected(SocketSession* session)
	{
		this->_session = session;
	}

	void onSocketDisConnect(SocketSession* session)
	{

	}

	void onSocketTimeout(SocketSession* session)
	{

	}

	void onSocketError(SocketSession* session)
	{

	}

public:
	void onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
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
		default:
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////////////////////////////

int __connect_center_server(const lw_char8* addr, lw_short16 port)
{
	PlatformClientHandler* cliHandler = new PlatformClientHandler();
	if (cliHandler->create(addr, port))
	{

	}

	return 0;
}

