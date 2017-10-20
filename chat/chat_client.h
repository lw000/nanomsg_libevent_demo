#ifndef __ChatClient_H__
#define __ChatClient_H__

#include <string>
#include <vector>

#include "common_type.h"
#include "common_struct.h"
#include "socket_client.h"
#include "GameClientHandler.h"

class SocketProcessor;
class SocketSession;

class ChatClient
{
private:

public:
	ChatClient();
	virtual ~ChatClient();

public:
	bool create(SocketConfig* conf);
	void destroy();

public:
	void sendData(lw_int32 cmd, void* object, lw_int32 objectSize);
	lw_int32 sendData(lw_int32 cmd, void* object, lw_int32 objectSize, const SendDataCallback& cb);

protected:
	void onSocketConnected(SocketSession* session);
	void onSocketDisConnect(SocketSession* session);
	void onSocketTimeout(SocketSession* session);
	void onSocketError(SocketSession* session);

protected:
	int onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize);

public:
	SocketClient* _cli;
};

#endif	//__ChatClient_H__

