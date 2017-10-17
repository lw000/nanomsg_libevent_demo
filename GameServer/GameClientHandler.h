#ifndef __GameHandler_H__
#define __GameHandler_H__

#include <string>
#include <vector>

#include "common_type.h"
#include "common_struct.h"

class AbstractGameClientHandler
{
public:
	virtual ~AbstractGameClientHandler() {}

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

class GameClientHandler : public AbstractGameClientHandler
{
	std::vector<USER_INFO> users;
	DESK_INFO _desk_info;

public:
	GameClientHandler();
	virtual ~GameClientHandler();

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

};

#endif	//__GameHandler_H__

