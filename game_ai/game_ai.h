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
class SocketTimer;

class GameAIMgr;
class GameAI;
class GameAIHandler;

class GameAI : public Threadable {
	friend class GameAIHandler;

private:
	GameAIMgr* _aimgr;

public:
	SocketTimer* _timer;
	std::vector<GameAIHandler*> _clis;

public:
	lw_fast_lock _cli_lock;

public:
	GameAI(GameAIMgr* aimgr);
	virtual ~GameAI();

public:
	void addClient(GameAIHandler* cli);
	void removeClient(GameAIHandler* cli);

protected:
	virtual int onStart() override;
	virtual int onRun() override;
	virtual int onEnd() override;
};


class GameAIMgr : public Threadable {
	friend class GameAI;
	friend class GameAIHandler;

public:
	GameAIMgr();
	virtual ~GameAIMgr();

protected:
	virtual int onStart() override;
	virtual int onRun() override;
	virtual int onEnd() override;

private:
	SocketTimer* _timer;
	SocketProcessor* _processor;
	GameAI*	_ai;
};


#endif	//__game_ai_h__

