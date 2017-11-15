/*
 * main.h
 *
 *  Created on: Sep 8, 2017
 *      Author: root
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <string>

typedef int(*MAIN_FUNC)(int, char**);

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
int main_db_server(int argc, char** argv);

#endif /* MAIN_H_ */
