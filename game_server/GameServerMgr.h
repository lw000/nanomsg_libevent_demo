#ifndef __GameServerMgr_H__
#define __GameServerMgr_H__

#include <string>
#include <vector>

#include "common_type.h"
#include "common_struct.h"
#include "GameClientHandler.h"
#include "lock.h"

class GameServerMgr
{
private:
	std::vector<AbstractGameClientHandler*> iDesks;
	lw_fast_lock _lock;

public:
	GameServerMgr();
	virtual ~GameServerMgr();

public:
	int create();
	void destroy();

public:
	void addGameHandler(AbstractGameClientHandler* desk);
	void removeGameHandler(AbstractGameClientHandler* desk);
};

#endif	//__GameServerMgr_H__

