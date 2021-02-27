#pragma once

#include "common.h"
#include "address.h"
#include "socket.h"

namespace twinkle
{
	bool startup()
	{
#if PLATFORM_WINDOWS
		WSADATA wsaData;
		WORD dllVersion = MAKEWORD(2, 2);
		return WSAStartup(dllVersion, &wsaData) == 0;
#else
		// Unix
#endif
	}

	bool shutdown()
	{
#if PLATFORM_WINDOWS 
		return WSACleanup() == 0;
#else 
		// Unix
#endif
	}
}