#ifndef __NetPackage_h__
#define __NetPackage_h__

#include <functional>
#include <iosfwd>
#include <iostream>
#include <string>

#include "NetHead.h"
#include "common_type.h"

namespace lwstar
{
	class NetPackage;

	class NetPackage
	{
	public:
		NetPackage();
		NetPackage(const NetHead* head);
		NetPackage(lw_int32 cmd, lw_void* msg = nullptr, lw_int32 size = 0);

	public:
		~NetPackage();

	public:
		lw_void setMessage(lw_char8* msg, lw_int32 size);
		lw_int32 setMessage(lw_int32 cmd, lw_void* msg, lw_int32 size);

	public:
		lw_char8* getBuf() const;
		lw_int32 getSize() const;
		const NetHead* getHead() const;

	public:
		std::string debug();

	private:
		NetHead _head;
		lw_char8 *_buf;
		lw_uint32 _size;
	};

};

#endif	//__NetPackage_h__
