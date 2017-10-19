#include "chat_client.h"

#include "common_marco.h"

#include "socket_processor.h"
#include "socket_session.h"
#include "socket_client.h"
#include "socket_config.h"
#include "client_session.h"

#include "command.h"
#include "platform.pb.h"
#include "chat.pb.h"


using namespace LW;

////////////////////////////////////////////////////////////////////////////////////////////////////

ChatClient::ChatClient()
{
	_cli = new SocketClient;
	_cli->connectedHandler = SOCKET_EVENT_SELECTOR(ChatClient::onSocketConnected, this);
	_cli->disConnectHandler = SOCKET_EVENT_SELECTOR(ChatClient::onSocketDisConnect, this);
	_cli->timeoutHandler = SOCKET_EVENT_SELECTOR(ChatClient::onSocketTimeout, this);
	_cli->errorHandler = SOCKET_EVENT_SELECTOR(ChatClient::onSocketError, this);
	_cli->parseHandler = SOCKET_PARSE_SELECTOR_4(ChatClient::onSocketParse, this);
}

ChatClient::~ChatClient()
{
	this->destroy();
	SAFE_DELETE(_cli);
}

bool ChatClient::create(SocketConfig* conf)
{
	bool ret = false;
	do 
	{
		ret = _cli->create(conf);
		if (!ret) {
			break;
		}

		ret = true;

	} while (0);

	return ret;
}

void ChatClient::destroy()
{
	_cli->destroy();
}

void ChatClient::sendData(lw_int32 cmd, void* object, lw_int32 objectSize)
{
	_cli->getSession()->sendData(cmd, object, objectSize);
}

lw_int32 ChatClient::sendData(lw_int32 cmd, void* object, lw_int32 objectSize, const SocketRecvHandlerConf& cb) {
	int c = _cli->getSession()->sendData(cmd, object, objectSize, cb);
	return c;
}

void ChatClient::onSocketConnected(SocketSession* session)
{
	_cli->addTimer(100, 10000, [this](int tid, unsigned int tms) -> bool {
		platform::msg_heartbeat msg;
		msg.set_time(time(NULL));
		int c = msg.ByteSizeLong();
		std::unique_ptr<char[]> s(new char[c]());
		bool ret = msg.SerializeToArray(s.get(), c);
		if (ret)
		{
			this->_cli->getSession()->sendData(cmd_heart_beat, s.get(), c);
		}
		return true;
	});
}

void ChatClient::onSocketDisConnect(SocketSession* session)
{
	_cli->close();
}

void ChatClient::onSocketTimeout(SocketSession* session)
{
	_cli->close();
}

void ChatClient::onSocketError(SocketSession* session)
{
	_cli->close();
}

int ChatClient::onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize)
{
	switch (cmd)
	{
	case sc_chat_reply: {

		break;
	}
	default: {
		break;
	}
	}

	return 0;
}
