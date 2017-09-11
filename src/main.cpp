//============================================================================
// Name        : main.cpp
// Author      : lw
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "main.h"

#include <string.h>
#include <stdio.h>

int main(int argc, char** argv) {
	if (argc < 2) return 0;

	if (strcmp(argv[1], "pubsub") == 0) {
		//pubsub tcp://127.0.0.1:5555 -s
		pubsub_servr_main(argc, argv);
	}
	else if (strcmp(argv[1], "center") == 0) {
		//center
		center_server_main(argc, argv);

	} if (strcmp(argv[1], "platform") == 0) {
		//platform
		platform_server_main(argc, argv);
	}
	else if (strcmp(argv[1], "http") == 0) {
		//http
		http_server_main(argc, argv);
	} else if (strcmp(argv[1], "game") == 0) {
		//game
		game_server_main(argc, argv);
	} else {
		fprintf(stderr, "usage: %s <url> [-s]\n", argv[0]);
	}

	return 0;
}
