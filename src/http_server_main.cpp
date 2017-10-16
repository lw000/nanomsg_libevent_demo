
#include "main.h"

#include <stdio.h>

#include <event2/thread.h>

#include "http_server_business.h"
#include "utils.h"

int main_http_server(int argc, char** argv)
{
	if (argc < 2) return 0;

#ifdef WIN32
	evthread_use_windows_threads();
#endif

	__create_http_service_business(9877);

	while (1) { lw_sleep(1); }

	return 0;
}

