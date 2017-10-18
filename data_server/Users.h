#ifndef __usermgr_h__
#define __usermgr_h__

#include <list>
#include "lock.h"
#include <string.h>

#include "common_struct.h"

class SocketSession;

struct UserSession
{
	UserInfo user;
	SocketSession* session;

public:
	UserSession()
	{
		this->reset();
	}

	void reset()
	{
		session = nullptr;
	}
};

class AbstractUser
{
public:
	virtual ~AbstractUser() {}

public:
	virtual int add(const UserInfo& user, SocketSession* session) = 0;

public:
	virtual void remove(int uid) = 0;
	virtual void remove(const UserInfo* user) = 0;
	virtual void remove(const SocketSession* session) = 0;

public:
	virtual const UserSession* find(int uid) = 0;
	virtual const UserSession* find(const UserInfo* user) = 0;
	virtual const UserSession* find(const SocketSession* session) = 0;

public:
	virtual void modify(UserInfo* user) = 0;

};

class Users : public AbstractUser
{
	typedef std::list<UserSession*> LIST_USER;
	typedef LIST_USER::iterator iterator;
	typedef LIST_USER::const_iterator const_iterator;
	typedef LIST_USER::reverse_iterator reverse_iterator;
	typedef LIST_USER::const_reverse_iterator const_reverse_iterator;
public:
	Users();
	virtual ~Users();

public:
	UserSession* operator[](int i);

public:
	virtual int add(const UserInfo& user, SocketSession* session) override;

public:
	virtual void remove(const SocketSession* session) override;
	virtual void remove(int uid) override;
	virtual void remove(const UserInfo* user) override;

public:
	virtual const UserSession* find(int uid) override;
	virtual const UserSession* find(const UserInfo* user) override;
	virtual const UserSession* find(const SocketSession* session) override;

public:
	virtual void modify(UserInfo* user) override;

public:
	void removeUserTest();

public:
	void restoreCache();

public:
	int size() const;

private:
	LIST_USER _alive;	//
	LIST_USER _free;	//
	lw_fast_lock _lock;
};

#endif	// !__usermgr_h__
