#include "NetPackage.h"
#include <time.h>
#include <stdlib.h>

#include <log4z.h>

#include "common_type.h"

namespace lwstar
{
	static const lw_int32 C_NETHEAD_SIZE = sizeof(NetHead);

	////////////////////////////////////////////////////////////////////////////////////////////////

	NetPackage::NetPackage() : _size(0)
	{
		_buf = NULL;
	}

	NetPackage::NetPackage(lw_int32 cmd, lw_void* msg, lw_int32 size) : NetPackage()
	{
		this->setMessage(cmd, msg, size);
	}

	NetPackage::NetPackage(const NetHead* head) : NetPackage()
	{
		if (head != NULL) {
			this->_head = *head;
		}
	}

	NetPackage::~NetPackage()
	{
		if (this->_buf != NULL)
		{
			free(this->_buf);
		}
	}

	lw_void NetPackage::setMessage(lw_char8* msg, lw_int32 size)
	{
		if (msg == NULL) return;
		if (size <= 0) return;

		this->_size = size;
		_buf = (lw_char8*)malloc(size * sizeof(lw_char8));
		memcpy(this->_buf, msg, size);
	}

	lw_int32 NetPackage::setMessage(lw_int32 cmd, lw_void* msg, lw_int32 size)
	{
		if ((msg != NULL) && (size <= 0)) {
			return -1;
		}

		do
		{
			this->_size = size + C_NETHEAD_SIZE;
			this->_head.size = this->_size;
			this->_head.cmd = cmd;
			this->_head.ctime = (lw_uint32)time(NULL);
			this->_buf = (lw_char8*)malloc(this->_size * sizeof(lw_char8));
			::memcpy(this->_buf, &this->_head, C_NETHEAD_SIZE);

			if ((NULL != msg) && (size > 0)) {
				::memcpy(&this->_buf[C_NETHEAD_SIZE], (void*)msg, size);
			}
			else {
				lw_char8 buf[256];
				sprintf(buf, "size: %d, NetHead: [cmd: %6d size: %6d ctime: %6d]", this->_size, this->_head.cmd, this->_head.size, this->_head.ctime);
				LOGD(buf);
			}
		} while (0);

		return 0;
	}

	std::string NetPackage::debug()
	{
		lw_char8 buf[256];
		sprintf(buf, "size: %d, NetHead: [cmd: %6d size: %6d ctime: %6d]", this->_size, this->_head.cmd, this->_head.size, this->_head.ctime);
		return std::string(buf);
	}

	char* NetPackage::getBuf() const
	{
		return this->_buf;
	}

	lw_int32 NetPackage::getSize() const
	{
		return this->_size;
	}

	const NetHead* NetPackage::getHead() const
	{
		return &this->_head;
	}
}
