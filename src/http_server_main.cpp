
#include "main.h"

#include <stdio.h>

#include <event2/thread.h>

#include "http_server_business.h"
#include "net.h"
#include "lw_util.h"

int http_server_main(int argc, char** argv)
{
	if (argc < 2) return 0;

	SocketInit s;

#ifdef WIN32
	evthread_use_windows_threads();
#endif

	__create_http_service_business(9877);

	while (1) { lw_sleep(1); }

	return 0;
}

