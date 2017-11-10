#include "UserServer.h"
#include <algorithm>
#include <time.h>

#include "socket_session.h"
#include "UserSession.h"

#include "log4z.h"


UserServer::UserServer()
{
}

UserServer::~UserServer()
{

}

const UserSession* UserServer::find(int uid)
{
	if (uid <= 0) {
		return nullptr;
	}

/*	clock_t t = clock();*/

	UserSession* pUsession = nullptr;
	{
		lw_fast_lock_guard l(_lock);
		iterator iter = this->_users.begin();
		for (; iter != this->_users.end(); ++iter)
		{
			if (uid == (*iter)->userinfo.uid)
			{
				pUsession = *iter;
				break;
			}
		}
	}

// 	clock_t t1 = clock();
// 	LOGFMTD("find uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);

	return pUsession;
}

const UserSession* UserServer::find(const UserInfo* user)
{
	if (user == nullptr)
	{
		return nullptr;
	}

	return this->find(user->uid);
}

const UserSession* UserServer::find(const UserSession* session)
{
	UserSession* pUsession = nullptr;

	{
		lw_fast_lock_guard l(_lock);
		iterator iter = this->_users.begin();
		for (; iter != this->_users.end(); ++iter)
		{
			if (session == (*iter))
			{
				pUsession = *iter;
				break;
			}
		}
	}

	return pUsession;
}

void UserServer::modify(UserInfo* user) {

}

int UserServer::add(UserSession* session)
{
	UserSession* pUsession = nullptr;

	{
		lw_fast_lock_guard l(_lock);

		{
/*			clock_t t = clock();*/
			iterator iter = this->_users.begin();
			for (; iter != this->_users.end(); ++iter)
			{
				if (session->userinfo.uid == (*iter)->userinfo.uid)
				{
					pUsession = *iter; break;
				}
			}
// 			clock_t t1 = clock();
// 			LOGFMTD("add -> find time[%f]", ((double)t1 - t) / CLOCKS_PER_SEC);
		}

		if (pUsession == nullptr)
		{
			this->_users.push_back(session);
		}
	}

	return 0;
}

void UserServer::remove(const UserSession* session)
{
	{
		lw_fast_lock_guard l(_lock);
		UserSession* pUsession = nullptr;
		iterator iter = this->_users.begin();
		for (; iter != this->_users.end(); ++iter)
		{
			if (session == (*iter))
			{
				pUsession = *iter;
				this->_users.erase(iter);

				break;
			}
		}
	}
}

void UserServer::remove(int uid)
{
	clock_t t = clock();
	{
		lw_fast_lock_guard l(_lock);
		UserSession* pUsession = nullptr;
		iterator iter = this->_users.begin();
		for (; iter != this->_users.end(); ++iter)
		{
			if (uid == (*iter)->userinfo.uid)
			{
				pUsession = *iter;
				this->_users.erase(iter);
				break;
			}
		}
	}
	clock_t t1 = clock();
	LOGFMTD("remove uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);
}

void UserServer::remove(const UserInfo* user)
{
	if (user == nullptr) return;
		
	this->remove(user->uid);
}

UserSession* UserServer::operator[](int i)
{
	if (i < 0)
	{
		return nullptr;
	}

	if (i > this->_users.size())
	{
		return nullptr;
	}

	UserSession* pUsession = nullptr;
	int j = 0;
	iterator iter = this->_users.begin();
	while (iter != this->_users.end())
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

void UserServer::removeUserTest()
{
	clock_t t = clock();
	int uid = 0;
	{
		lw_fast_lock_guard l(_lock);

		UserSession* pUsession = nullptr;

		if (!this->_users.empty())
		{
			pUsession = this->_users.front();
			this->_users.pop_front();
		}
	}
	clock_t t1 = clock();
	LOGFMTD("remove uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);
}

int UserServer::size() const {
	return this->_users.size();
}

//
//UserManager::UserManager()
//{
//}
//
//UserManager::~UserManager()
//{
//	{
//		lw_lock_guard l(&_lock);
//		iterator iter = _alive.begin();
//		for (; iter != _alive.end(); ++iter)
//		{
//			UserSession* pUsession = (*iter);
//			{
//				delete pUsession;
//				pUsession = nullptr;
//			}
//			delete pUsession;
//		}
//	}
//
//	{
//		lw_lock_guard l(&_lock);
//		iterator iter = _free.begin();
//		for (; iter != _free.end(); ++iter)
//		{
//			UserSession* pUsession = (*iter);
//			delete pUsession;
//			pUsession = nullptr;
//		}
//	}
//}
//
//const UserSession* UserManager::find(int uid)
//{
//	if (uid <= 0) {
//		return nullptr;
//	}
//	
//	clock_t t = clock();
//	
//	UserSession* pUsession = nullptr;
//	{
//		lw_lock_guard l(&_lock);
//		iterator iter = _alive.begin();
//		for (; iter != _alive.end(); ++iter)
//		{
//			if (uid == (*iter)->userinfo.uid)
//			{
//				pUsession = *iter;
//				break;
//			}
//		}
//	}
//
//	clock_t t1 = clock();
//	LOGFMTD("find uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);
//
//	return pUsession;
//}
//
//const UserSession* UserManager::find(const UserInfo* user)
//{
//	if (user == nullptr)
//	{
//		return nullptr;		
//	}
//
//	return this->find(user->uid);
//}
//
//const UserSession* UserManager::find(const UserSession* session)
//{
//	UserSession* pUsession = nullptr;
//	
//	{
//		lw_lock_guard l(&_lock);
//		iterator iter = _alive.begin();
//		for (; iter != _alive.end(); ++iter)
//		{
//			if (session == (*iter))
//			{
//				pUsession = *iter;
//				break;
//			}
//		}
//	}
//
//	return pUsession;
//}
//
//void UserManager::modify(UserInfo* user) {
//
//}
//
//int UserManager::add(UserSession* session)
//{
//	// 1. If the user does not exist, create an object and add a user
//	// 2. If users exist, update user information
//	
//	UserSession* pUsession = nullptr;
//
//	{
//		lw_lock_guard l(&_lock);
//
//		{
//			clock_t t = clock();
//
//#if 1
//			iterator iter = _alive.begin();
//			for (; iter != _alive.end(); ++iter)
//			{
//				if (session->userinfo.uid == (*iter)->userinfo.uid)
//				{
//					pUsession = *iter; break;
//				}
//			}
//#else
//			reverse_iterator iter = _alive.rbegin();
//			for (; iter != _alive.rend(); ++iter)
//			{
//				if (user.uid == (*iter)->user.uid)
//				{
//					pUsession = *iter; break;
//				}
//			}
//#endif
//			clock_t t1 = clock();
//			LOGFMTD("add -> find time[%f]", ((double)t1 - t) / CLOCKS_PER_SEC);
//		}
//
//		if (pUsession == nullptr)
//		{
//			if (!_free.empty())
//			{
//				pUsession = _free.front();
//				_free.pop_front();
//			}
//
//			if (pUsession == nullptr)
//			{
//				pUsession = new UserSession;
//			}
//
//			_alive.push_back(pUsession);
//		}
//
//		pUsession = session;
//
//// 		{
//// 			clock_t t = clock();
//// 
//// 			_alive.sort([](UserSession* s1, UserSession* s2) -> bool {
//// 				return s1->user.uid > s2->user.uid;
//// 			});
//// 
//// 			clock_t t1 = clock();
//// 			LOGFMTD("add -> sort time[%f]", ((double)t1 - t) / CLOCKS_PER_SEC);
//// 		}
//	}
//
//	return 0;
//}
//
//void UserManager::remove(const UserSession* session)
//{
//	clock_t t = clock();
//	int uid = 0;
//	{
//		lw_lock_guard l(&_lock);
//		UserSession* pUsession = nullptr;
//		iterator iter = _alive.begin();
//		for (; iter != _alive.end(); ++iter)
//		{
//			if (session == (*iter))
//			{
//				pUsession = *iter;
//				_alive.erase(iter);
//				
//				break;
//			}
//		}
//
//		if (pUsession != nullptr)
//		{
//			uid = pUsession->userinfo.uid;
//
//			pUsession->reset();
//			_free.push_back(pUsession);
//		}
//	}
//	clock_t t1 = clock();
//	LOGFMTD("remove uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);
//}
//
//void UserManager::remove(int uid)
//{
//	clock_t t = clock();
//	{
//		lw_lock_guard l(&_lock);
//		UserSession* pUsession = nullptr;
//		iterator iter = _alive.begin();
//		for (; iter != _alive.end(); ++iter)
//		{
//			if (uid == (*iter)->userinfo.uid)
//			{
//				pUsession = *iter;
//				_alive.erase(iter);
//				break;
//			}
//		}
//
//		if (pUsession != nullptr)
//		{
//			pUsession->reset();
//			_free.push_back(pUsession);
//		}
//	}
//	clock_t t1 = clock();
//	LOGFMTD("remove uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);
//}
//
//void UserManager::remove(const UserInfo* user)
//{
//	if (user != nullptr)
//	{
//		return this->remove(user->uid);
//	}
//}
//
//UserSession* UserManager::operator[](int i)
//{
//	if ((i < 0) || (i > _alive.size()))
//	{
//		return nullptr;
//	}
//
//	UserSession* pUsession = nullptr;
//	int j = 0;
//	iterator iter = _alive.begin();
//	while (iter != _alive.end())
//	{
//		if (j == i)
//		{
//			pUsession = *iter;
//			break;
//		}
//		++iter;
//	}
//
//	return pUsession;
//}
//
//void UserManager::removeUserTest()
//{
//	clock_t t = clock();
//	int uid = 0;
//	{
//		lw_lock_guard l(&_lock);
//
//		UserSession* pUsession = nullptr;
//
//		// ������ȡ������
//		if (!_alive.empty())
//		{
//			pUsession = _alive.front();
//			_alive.pop_front();
//		}
//
//		if (pUsession != nullptr)
//		{
//			uid = pUsession->userinfo.uid;
//
//			pUsession->reset();
//			_free.push_back(pUsession);
//		}
//	}
//	clock_t t1 = clock();
//	LOGFMTD("remove uid = [%d], time [%f]", uid, ((double)t1 - t) / CLOCKS_PER_SEC);
//}
//
//void UserManager::restoreCache()
//{
//	{
//		lw_lock_guard l(&_lock);
//
//		iterator iter = _free.begin();
//		for (; iter != _free.end(); ++iter)
//		{
//			UserSession* pUsession = (*iter);
//			delete pUsession;
//			pUsession = nullptr;
//		}
//
//		{			
//			LIST_USER().swap(_free);
//		}
//	}
//}
//
//int UserManager::size() const {
//	return _alive.size();
//}
