#ifndef __GameLogic_H__
#define __GameLogic_H__

#include <string>
#include <vector>

#include "common_type.h"
#include "common_struct.h"
#include "socket_client.h"

class SocketProcessor;
class SocketSession;

class AbstractGameServer
{
public:
	virtual ~AbstractGameServer() {}

public:
	virtual bool create(const DESK_INFO& info) = 0;
	virtual void destroy() = 0;

public:
	virtual void onGameStartReponse(void* data, int datasize) = 0;
	virtual void onGameEndReponse(void* data, int datasize) = 0;
	virtual void onGameUserSitupReponse(void* data, int datasize) = 0;
	virtual void onGameUserSitdownReponse(void* data, int datasize) = 0;

public:
	virtual int onGameMessage(int cmd, void* data, int datasize) = 0;
};

class GameServer : public AbstractGameServer
{
private:
	AbstractGameServer* iDesk;

private:
	DESK_INFO _desk_info;
	std::vector<USER_INFO> users;

public:
	SocketEventHandler connectedHandler;
	SocketEventHandler disConnectHandler;
	SocketEventHandler timeoutHandler;
	SocketEventHandler errorHandler;

public:
	GameServer(AbstractGameServer* idesk);
	virtual ~GameServer();

public:
	void start();

public:
	void sendSitup();
	void sendSitdown();

public:
	void sendData(lw_int32 cmd, void* object, lw_int32 objectSize);

protected:
	int onSocketConnected(SocketSession* session);
	int onSocketDisConnect(SocketSession* session);
	int onSocketTimeout(SocketSession* session);
	int onSocketError(SocketSession* session);

	void onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize);

public:
	int frameMessage(int cmd, void* data, int datasize);

public:
	virtual bool create(const DESK_INFO& info) override;
	virtual void destroy() override;

public:
	virtual void onGameStartReponse(void* data, int datasize) override;
	virtual void onGameEndReponse(void* data, int datasize) override;
	virtual void onGameUserSitupReponse(void* data, int datasize) override;
	virtual void onGameUserSitdownReponse(void* data, int datasize) override;

public:
	virtual int onGameMessage(int cmd, void* data, int datasize) override;

public:
	SocketClient* client;
};

#endif	//__GameLogic_H__

