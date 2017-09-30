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
	
	START_ENTER_METHOD METHODS[] = {
		{ "center", main_center_server },
		{ "platform", main_platform_server },
		{ "game", main_game_server},
		{ "http", main_http_server },
		{ "pubsub", main_pubsub_servr },	//pubsub tcp://127.0.0.1:5555 -s
		{ "survey", main_nanomsg_survey },
		{ "nanorpc", main_nonamsg_rpc },
	};

	std::string s(argv[1]);

	for (int i = 0; i < sizeof(METHODS) / sizeof(METHODS[0]); i++) {
		if (s.compare(METHODS[i].name) == 0) {
			
			if (METHODS[i].call != NULL) {
				METHODS[i].call(argc, argv);
			}

			break;
		}
	}

	return 0;
}
