#ifndef __GameClientHandler_H__
#define __GameClientHandler_H__

#include <string>
#include <vector>

#include "common_type.h"
#include "common_struct.h"

class AbstractGameClientHandler
{
public:
	virtual ~AbstractGameClientHandler() {}

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
	std::vector<UserInfo> users;
	DeskInfo _desk_info;

public:
	GameClientHandler(const DeskInfo& info);
	virtual ~GameClientHandler();

public:
	virtual void onGameStartReponse(void* data, int datasize) override;
	virtual void onGameEndReponse(void* data, int datasize) override;
	virtual void onGameUserSitupReponse(void* data, int datasize) override;
	virtual void onGameUserSitdownReponse(void* data, int datasize) override;

public:
	virtual int onGameMessage(int cmd, void* data, int datasize) override;

};

#endif	//__GameClientHandler_H__

