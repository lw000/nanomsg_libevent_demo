#ifndef __game_ai_h__
#define __game_ai_h__

#include <string>
#include <vector>

#include "common_type.h"
#include "common_struct.h"
#include "Threadable.h"
#include "lock.h"

class SocketProcessor;
class SocketSession;
class Timer;

class GameAIMgr;

static const int GAME_AI_COUNT = 100;

class GameAIMgr : public Threadable {
	friend class GameAiClientHandler;

public:
	GameAIMgr();
	virtual ~GameAIMgr();

protected:
	virtual int onStart() override;
	virtual int onRun() override;
	virtual int onEnd() override;

private:
	bool _is_init;
	Timer* _timer;
	SocketProcessor* _processor;
	SocketSession* sessions[GAME_AI_COUNT];
	lw_fast_lock _lock;
};


#endif	//__game_ai_h__

