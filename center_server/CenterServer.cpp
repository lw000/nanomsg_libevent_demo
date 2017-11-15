#include "CenterServer.h"

#include "command.h"
#include "platform.pb.h"

#include "log4z.h"
#include "socket_session.h"
#include "socket_config.h"
#include "server_session.h"
#include "common_marco.h"

using namespace LW;

CenterServerHandler::CenterServerHandler() {
	this->listenHandler = SOCKET_LISTENER_SELECTOR_2(
			CenterServerHandler::onSocketListener, this);
	this->listenErrorHandler = SOCKET_LISTENER_ERROR_SELECTOR_2(
			CenterServerHandler::onSocketListenerError, this);
}

CenterServerHandler::~CenterServerHandler() {

}

SocketSession* CenterServerHandler::onSocketListener(SocketProcessor* processor,
evutil_socket_t fd) {
	ServerSession* pSession = new ServerSession;
	if (pSession != nullptr) {
		pSession->onDisconnectHandler =
				[pSession, this](SocketSession* session) {
					pSession->onDisconnectHandler(session);
					this->Sessions.remove(session);
				};
		pSession->onTimeoutHandler = [pSession, this](SocketSession* session) {
			pSession->onTimeoutHandler(session);
			this->Sessions.remove(session);
		};
		pSession->onErrorHandler = [pSession, this](SocketSession* session) {
			pSession->onErrorHandler(session);
			this->Sessions.remove(session);
		};
		pSession->onDataParseHandler =
				[pSession, this](SocketSession* session, lw_int32 cmd, lw_char8* buf, lw_int32 bufsize) -> int {
					int c = pSession->onDataParseHandler(session, cmd, buf, bufsize);
					return c;
				};

		int r = pSession->create(processor, new SocketConfig, fd);
		if (r == 0) {

			Sessions.add(pSession);

			platform::msg_connected msg;
			msg.set_time(time(NULL));
			{
				std::string s;
				bool ret = msg.SerializeToString(&s);
				if (ret) {
					pSession->sendData(cmd_connected, (void*) s.c_str(),
							s.size());
				}
			}
		}
		else {
			pSession->destroy();
			SAFE_DELETE(pSession);
		}
	}
	return pSession;
}

void CenterServerHandler::onSocketListenerError(void * userdata, int er) {
	LOGFMTD("got an error %d (%s) on the listener. shutting down.\n", er,
			evutil_socket_error_to_string(er));
	this->close();
}
