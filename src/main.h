/*
 * main.h
 *
 *  Created on: Sep 8, 2017
 *      Author: root
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <string>

typedef int(*ENTER_FUN)(int, char**);

typedef struct tagSTART_ENTER_METHOD
{
	std::string name;
	ENTER_FUN call;
} START_ENTER_METHOD;

int main_center_server(int argc, char** argv);
int main_platform_server(int argc, char** argv);
int main_http_server(int argc, char** argv);
int main_game_server(int argc, char** argv);
int main_pubsub_servr(int argc, char** argv);
int main_nanomsg_survey(int argc, char** argv);
int main_nonamsg_rpc(int argc, char** argv);
int main_ai_server(int argc, char** argv);
int main_chat_server(int argc, char** argv);
int main_timer_server(int argc, char** argv);
int main_http_client(int argc, char** argv);

#endif /* MAIN_H_ */
