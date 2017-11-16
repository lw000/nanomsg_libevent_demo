#ifndef __GameMgr_H__
#define __GameMgr_H__

#include <string>
#include <vector>

#include "common_type.h"
#include "common_struct.h"
#include "socket_server.h"

class SocketProcessor;
class SocketSession;
class AbstractGameServerHandler;

class GameMgr {
		std::vector<AbstractGameServerHandler*> _games;

	public:
		GameMgr();
		virtual ~GameMgr();

	public:
		bool create(const RoomInfo& r);
		void destroy();

	public:
		void sendSitup(int uid);
		void sendSitdown(int uid);

	public:
		void sendData(lw_int32 cmd, void* object, lw_int32 objectSize);
		lw_int32 sendData(lw_int32 cmd, void* object, lw_int32 objectSize,
				const SendDataCallback& cb);

	protected:
		void onSocketConnected(SocketSession* session);
		void onSocketDisConnect(SocketSession* session);
		void onSocketTimeout(SocketSession* session);
		void onSocketError(SocketSession* session);
		int onSocketParse(SocketSession* session, lw_int32 cmd, lw_char8* buf,
				lw_int32 bufsize);

	protected:
		int frameMessage(int cmd, void* data, int datasize);
		int onGameMessage(int cmd, void* data, int datasize);

};

#endif	//__GameMgr_H__

