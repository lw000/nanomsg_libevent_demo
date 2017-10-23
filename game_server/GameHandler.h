#ifndef __GameHandler_H__
#define __GameHandler_H__

#include <string>
#include <vector>

#include "common_type.h"
#include "common_struct.h"

class AbstractGameServerHandler
{
public:
	virtual ~AbstractGameServerHandler() {}

public:
	virtual void onGameStartReponse(void* data, int datasize) = 0;
	virtual void onGameEndReponse(void* data, int datasize) = 0;
	virtual void onGameUserSitupReponse(void* data, int datasize) = 0;
	virtual void onGameUserSitdownReponse(void* data, int datasize) = 0;

public:
	virtual int onGameMessage(int cmd, void* data, int datasize) = 0;
};

class GameHandler : public AbstractGameServerHandler
{
	std::vector<UserInfo> users;
	DeskInfo _desk_info;

public:
	GameHandler(const DeskInfo& info);
	virtual ~GameHandler();

public:
	virtual void onGameStartReponse(void* data, int datasize) override;
	virtual void onGameEndReponse(void* data, int datasize) override;
	virtual void onGameUserSitupReponse(void* data, int datasize) override;
	virtual void onGameUserSitdownReponse(void* data, int datasize) override;

public:
	virtual int onGameMessage(int cmd, void* data, int datasize) override;

};

#endif	//__GameHandler_H__

