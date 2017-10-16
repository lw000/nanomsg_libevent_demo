#include "Users.h"
#include <algorithm>

#include "log4z.h"

#include "socket_session.h"

using namespace zsummer::log4z;

Users::Users()
{
}

Users::~Users()
{
	{
		iterator iter = _live.begin();
		for (; iter != _live.end(); ++iter)
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
		iterator iter = _cache.begin();
		for (; iter != _cache.end(); ++iter)
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
	UserSession* pUsession = nullptr;
	{
		lw_lock_guard l(&_lock);
		iterator iter = _live.begin();
		for (; iter != _live.end(); ++iter)
		{
			if (uid == (*iter)->user.uid)
			{
				pUsession = *iter;
				break;
			}
		}
	}
	
	return pUsession;
}

const UserSession* Users::find(const USER_INFO* user)
{
	if (user != nullptr)
	{
		return this->find(user->uid);
	}

	return nullptr;
}

const UserSession* Users::find(const SocketSession* session)
{
	UserSession* pUsession = nullptr;
	{
		lw_lock_guard l(&_lock);
		iterator iter = _live.begin();
		for (; iter != _live.end(); ++iter)
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

int Users::add(const USER_INFO& user, SocketSession* session)
{
	// ����û������ڣ�������������û�
	// ����û����ڣ������û���Ϣ
	int r = 0;

// 	clock_t t = clock();
// 	int uid = 0;
	{
		UserSession* pUsession = nullptr;

		{
			lw_lock_guard l(&_lock);
			iterator iter = _live.begin();
			for (; iter != _live.end(); ++iter)
			{
				if (user.uid == (*iter)->user.uid)
				{
					pUsession = *iter; break;
				}
			}

			if (pUsession == nullptr)
			{
				// ������ȡ������
				if (!_cache.empty())
				{
					pUsession = _cache.front();
					_cache.pop_front();
				}

				if (pUsession == nullptr)
				{
					pUsession = new UserSession;
				}
				_live.push_back(pUsession);
			}

			pUsession->session = session;
			pUsession->user = user;
/*			uid = pUsession->user.uid;*/
		}
	}

// 	clock_t t1 = clock();
// 	{
// 		char s[512];
// 		sprintf(s, "add uid = [%d], time [%f] \n", uid, ((double)t1 - t) / CLOCKS_PER_SEC);
// 		LOGD(s);
// 	}
	
	return r;
}

void Users::remove(const SocketSession* session)
{
	clock_t t = clock();
	int uid = 0;
	{
		lw_lock_guard l(&_lock);
		UserSession* pUsession = nullptr;
		iterator iter = _live.begin();
		for (; iter != _live.end(); ++iter)
		{
			if (session == (*iter)->session)
			{
				pUsession = *iter;
				_live.erase(iter);
				
				break;
			}
		}

		if (pUsession != nullptr)
		{
			uid = pUsession->user.uid;

			pUsession->reset();
			_cache.push_back(pUsession);
		}
	}
	clock_t t1 = clock();
	{
		char s[512];
		sprintf(s, "remove uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);
		LOGD(s);
	}
}

void Users::remove(int uid)
{
	clock_t t = clock();
	{
		lw_lock_guard l(&_lock);
		UserSession* pUsession = nullptr;
		iterator iter = _live.begin();
		for (; iter != _live.end(); ++iter)
		{
			if (uid == (*iter)->user.uid)
			{
				pUsession = *iter;
				_live.erase(iter);
				break;
			}
		}

		if (pUsession != nullptr)
		{
			pUsession->reset();
			_cache.push_back(pUsession);
		}
	}
	clock_t t1 = clock();
	{
		char s[512];
		sprintf(s, "remove uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);
		LOGD(s);
	}
}

void Users::remove(const USER_INFO* user)
{
	if (user != nullptr)
	{
		return this->remove(user->uid);
	}
}

UserSession* Users::operator[](int i)
{
	if ((i < 0) || (i > _live.size()))
	{
		return nullptr;
	}

	UserSession* pUsession = nullptr;
	int j = 0;
	iterator iter = _live.begin();
	while (iter != _live.end())
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
		if (!_live.empty())
		{
			pUsession = _live.front();
			_live.pop_front();
		}

		if (pUsession != nullptr)
		{
			uid = pUsession->user.uid;

			pUsession->reset();
			_cache.push_back(pUsession);
		}
	}
	clock_t t1 = clock();
	{
		char s[512];
		sprintf(s, "remove uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);
		LOGD(s);
	}
}

void Users::restoreCache()
{
	{
		lw_lock_guard l(&_lock);

		iterator iter = _cache.begin();
		for (; iter != _cache.end(); ++iter)
		{
			UserSession* pUsession = (*iter);
			{
				delete pUsession->session;
				pUsession->session = nullptr;
			}
			delete pUsession;
		}

		{			
			USER_LIST().swap(_cache);
		}
	}
}

