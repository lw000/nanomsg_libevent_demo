#include "Users.h"
#include <algorithm>

#include "log4z.h"

#include "socket_session.h"
#include <time.h>

Users::Users()
{
}

Users::~Users()
{
	{
		lw_lock_guard l(&_lock);
		iterator iter = _alive.begin();
		for (; iter != _alive.end(); ++iter)
		{
			UserSession* pUsession = (*iter);
			{
				delete pUsession->session;
				pUsession->session = nullptr;
			}
			delete pUsession;
		}
	}

	{
		lw_lock_guard l(&_lock);
		iterator iter = _free.begin();
		for (; iter != _free.end(); ++iter)
		{
			UserSession* pUsession = (*iter);
			{
				delete pUsession->session;
				pUsession->session = nullptr;
			}
			delete pUsession;
		}
	}
}

const UserSession* Users::find(int uid)
{
	if (uid <= 0) {
		return nullptr;
	}
	
	clock_t t = clock();
	
	UserSession* pUsession = nullptr;
	{
		lw_lock_guard l(&_lock);
		iterator iter = _alive.begin();
		for (; iter != _alive.end(); ++iter)
		{
			if (uid == (*iter)->user.uid)
			{
				pUsession = *iter;
				break;
			}
		}
	}

	clock_t t1 = clock();
	LOGFMTD("find uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);

	return pUsession;
}

const UserSession* Users::find(const UserInfo* user)
{
	if (user == nullptr)
	{
		return nullptr;		
	}

	return this->find(user->uid);
}

const UserSession* Users::find(const SocketSession* session)
{
	UserSession* pUsession = nullptr;
	
	{
		lw_lock_guard l(&_lock);
		iterator iter = _alive.begin();
		for (; iter != _alive.end(); ++iter)
		{
			if (session == (*iter)->session)
			{
				pUsession = *iter;
				break;
			}
		}
	}

	return pUsession;
}

void Users::modify(UserInfo* user) {

}

int Users::add(const UserInfo& user, SocketSession* session)
{
	// 1. If the user does not exist, create an object and add a user
	// 2. If users exist, update user information
	
	UserSession* pUsession = nullptr;

	{
		lw_lock_guard l(&_lock);

		{
			clock_t t = clock();

#if 1
			iterator iter = _alive.begin();
			for (; iter != _alive.end(); ++iter)
			{
				if (user.uid == (*iter)->user.uid)
				{
					pUsession = *iter; break;
				}
			}
#else
			reverse_iterator iter = _alive.rbegin();
			for (; iter != _alive.rend(); ++iter)
			{
				if (user.uid == (*iter)->user.uid)
				{
					pUsession = *iter; break;
				}
			}
#endif
			clock_t t1 = clock();
			LOGFMTD("add -> find time[%f]", ((double)t1 - t) / CLOCKS_PER_SEC);
		}

		if (pUsession == nullptr)
		{
			if (!_free.empty())
			{
				pUsession = _free.front();
				_free.pop_front();
			}

			if (pUsession == nullptr)
			{
				pUsession = new UserSession;
			}
			_alive.push_back(pUsession);
		}

		pUsession->session = session;
		pUsession->user = user;

// 		{
// 			clock_t t = clock();
// 
// 			_alive.sort([](UserSession* s1, UserSession* s2) -> bool {
// 				return s1->user.uid > s2->user.uid;
// 			});
// 
// 			clock_t t1 = clock();
// 			LOGFMTD("add -> sort time[%f]", ((double)t1 - t) / CLOCKS_PER_SEC);
// 		}
	}

	return 0;
}

void Users::remove(const SocketSession* session)
{
	clock_t t = clock();
	int uid = 0;
	{
		lw_lock_guard l(&_lock);
		UserSession* pUsession = nullptr;
		iterator iter = _alive.begin();
		for (; iter != _alive.end(); ++iter)
		{
			if (session == (*iter)->session)
			{
				pUsession = *iter;
				_alive.erase(iter);
				
				break;
			}
		}

		if (pUsession != nullptr)
		{
			uid = pUsession->user.uid;

			pUsession->reset();
			_free.push_back(pUsession);
		}
	}
	clock_t t1 = clock();
	LOGFMTD("remove uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);
}

void Users::remove(int uid)
{
	clock_t t = clock();
	{
		lw_lock_guard l(&_lock);
		UserSession* pUsession = nullptr;
		iterator iter = _alive.begin();
		for (; iter != _alive.end(); ++iter)
		{
			if (uid == (*iter)->user.uid)
			{
				pUsession = *iter;
				_alive.erase(iter);
				break;
			}
		}

		if (pUsession != nullptr)
		{
			pUsession->reset();
			_free.push_back(pUsession);
		}
	}
	clock_t t1 = clock();
	LOGFMTD("remove uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);
}

void Users::remove(const UserInfo* user)
{
	if (user != nullptr)
	{
		return this->remove(user->uid);
	}
}

UserSession* Users::operator[](int i)
{
	if ((i < 0) || (i > _alive.size()))
	{
		return nullptr;
	}

	UserSession* pUsession = nullptr;
	int j = 0;
	iterator iter = _alive.begin();
	while (iter != _alive.end())
	{
		if (j == i)
		{
			pUsession = *iter;
			break;
		}
		++iter;
	}

	return pUsession;
}

void Users::removeUserTest()
{
	clock_t t = clock();
	int uid = 0;
	{
		lw_lock_guard l(&_lock);

		UserSession* pUsession = nullptr;

		// ������ȡ������
		if (!_alive.empty())
		{
			pUsession = _alive.front();
			_alive.pop_front();
		}

		if (pUsession != nullptr)
		{
			uid = pUsession->user.uid;

			pUsession->reset();
			_free.push_back(pUsession);
		}
	}
	clock_t t1 = clock();
	LOGFMTD("remove uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);
}

void Users::restoreCache()
{
	{
		lw_lock_guard l(&_lock);

		iterator iter = _free.begin();
		for (; iter != _free.end(); ++iter)
		{
			UserSession* pUsession = (*iter);
			{
				delete pUsession->session;
				pUsession->session = nullptr;
			}
			delete pUsession;
		}

		{			
			LIST_USER().swap(_free);
		}
	}
}

int Users::size() const {
	return _alive.size();
}
