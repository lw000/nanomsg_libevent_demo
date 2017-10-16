
#include "main.h"

#include <stdio.h>

#include <event2/thread.h>

#include "http_server_business.h"
#include "utils.h"

int main_http_server(int argc, char** argv)
{
	__create_http_service_business(9877);

	return 0;
}

