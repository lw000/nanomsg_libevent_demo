#ifndef __usermgr_h__
#define __usermgr_h__

#include <list>
#include "lock.h"
#include <string.h>

#include "common_struct.h"

class SocketSession;
class UserSession;

class AbstractUserManager
{
public:
	virtual ~AbstractUserManager() {}

public:
	virtual int onAdd(UserSession* session) = 0;

public:
	virtual void onRemove(int uid) = 0;
	virtual void onRemove(const UserInfo* user) = 0;
	virtual void onRemove(const UserSession* session) = 0;

public:
	virtual void onUpdate(UserInfo* user) = 0;
};

class UserServer
{
	typedef std::list<UserSession*> OPT_USER;
	typedef OPT_USER::iterator iterator;
	typedef OPT_USER::const_iterator const_iterator;
	typedef OPT_USER::reverse_iterator reverse_iterator;
	typedef OPT_USER::const_reverse_iterator const_reverse_iterator;

public:
	UserServer();
	virtual ~UserServer();

public:
	UserSession* operator[](int i);

public:
	int add(UserSession* session);

public:
	void remove(const UserSession* session);
	void remove(int uid);
	void remove(const UserInfo* user);

public:
	const UserSession* find(int uid);
	const UserSession* find(const UserInfo* user);
	const UserSession* find(const UserSession* session);

public:
	void modify(UserInfo* user);

public:
	void removeUserTest();

public:
	int size() const;

private:
	OPT_USER _users;
	lw_fast_mutex _lock;
};

//
//class UserManager : public AbstractUserManager
//{
//	typedef std::list<UserSession*> LIST_USER;
//	typedef LIST_USER::iterator iterator;
//	typedef LIST_USER::const_iterator const_iterator;
//	typedef LIST_USER::reverse_iterator reverse_iterator;
//	typedef LIST_USER::const_reverse_iterator const_reverse_iterator;
//
//public:
//	UserManager();
//	virtual ~UserManager();
//
//public:
//	UserSession* operator[](int i);
//
//public:
//	virtual int add(UserSession* session) override;
//
//public:
//	virtual void remove(const UserSession* session) override;
//	virtual void remove(int uid) override;
//	virtual void remove(const UserInfo* user) override;
//
//public:
//	virtual const UserSession* find(int uid) override;
//	virtual const UserSession* find(const UserInfo* user) override;
//	virtual const UserSession* find(const UserSession* session) override;
//
//public:
//	virtual void modify(UserInfo* user) override;
//
//public:
//	void removeUserTest();
//
//public:
//	void restoreCache();
//
//public:
//	int size() const;
//
//private:
//	LIST_USER _alive;	//
//	LIST_USER _free;	//
//	lw_fast_lock _lock;
//};

#endif	// !__usermgr_h__
