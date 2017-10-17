#include "GameServerMgr.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////////////////////////

GameServerMgr::GameServerMgr()
{

}

GameServerMgr::~GameServerMgr()
{
	
}

int GameServerMgr::create() {

	return 0;
}

void GameServerMgr::destroy() {
	{
		lw_lock_guard l(&_lock);

		std::for_each(this->iDesks.begin(), this->iDesks.end(), [](AbstractGameClientHandler* c) {
			c->destroy();
			delete c;
		});
	}
}

void GameServerMgr::addGameHandler(AbstractGameClientHandler* desk) {
	{
		lw_lock_guard l(&_lock);
		std::vector<AbstractGameClientHandler*>::iterator iter = std::find_if(this->iDesks.begin(), this->iDesks.end(), [desk](AbstractGameClientHandler* c) -> bool {
			return (c == desk);
		});

		if (iter == this->iDesks.end()) {
			this->iDesks.push_back(desk); 
		}
		else {
			*iter = desk;
		}
	}
}

void GameServerMgr::removeGameHandler(AbstractGameClientHandler* desk) {
	{
		lw_lock_guard l(&_lock);
		std::vector<AbstractGameClientHandler*>::iterator iter = std::find_if(this->iDesks.begin(), this->iDesks.end(), [desk](AbstractGameClientHandler* c) -> bool {
			return (c == desk);
		});

		if (iter != this->iDesks.end()) {
			delete *iter;

			this->iDesks.erase(iter);
		}
	}
}