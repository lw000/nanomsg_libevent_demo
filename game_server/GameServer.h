#ifndef __GameServer_H__
#define __GameServer_H__

#include <string>
#include <vector>

#include "common_type.h"
#include "common_struct.h"
#include "socket_client.h"
#include "GameHandler.h"

class SocketProcessor;
class SocketSession;
class SocketClient;

class GameServer
{
private:
	AbstractGameServerHandler* _gameHandler;

public:
	GameServer(AbstractGameServerHandler* handler);
	virtual ~GameServer();

public:
	bool create();
	void destroy();

public:
	void sendSitup(int uid);
	void sendSitdown(int uid);

public:
	void sendData(lw_int32 cmd, void* object, lw_int32 objectSize);
	lw_int32 sendData(lw_int32 cmd, void* object, lw_int32 objectSize, const SendDataCallback& cb);

protected:
	void onSocketConnected(SocketSession* session);
	void onSocketDisConnect(SocketSession* session);
	void onSocketTimeout(SocketSession* session);
	void onSocketError(SocketSession* session);
	int onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize);

protected:
	int frameMessage(int cmd, void* data, int datasize);
	int onGameMessage(int cmd, void* data, int datasize);
};

#endif	//__GameServer_H__

