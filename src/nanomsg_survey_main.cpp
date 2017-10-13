#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <string>

#include "nn.h"
#include "survey.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "nanomsgcpp_socket.h"
#include "Threadable.h"

class SurveyServer;
class SurveyClient;

struct AddRequest {
	int a;
	int b;
};

struct AddReponse {
	int c;
};

class SurveyServer : public NanomsgcppSocket, public Threadable
{
public:
	SurveyServer() {
	}

	virtual ~SurveyServer() {
	}

public:
	virtual int onStart() {
		return 0;
	}

	virtual int onRun() {

		const char *url = "tcp://127.0.0.1:8888";
		int sock = this->create(AF_SP, NN_SURVEYOR);
		if (sock < 0) {
			fprintf(stderr, "nn_socket failed: %s \n", nn_strerror(errno));
			return -1;
		}

		if (this->bind(url) < 0) {
			fprintf(stderr, "nn_bind fail: %s \n", nn_strerror(errno));
			return -1;
		}

		AddRequest add_r = { 10, 20 };
		int sendSize = sizeof(add_r);
		while (1) {
			int c = this->send(1, &add_r, sendSize);
			if (c < 0) {
				fprintf(stderr, "send failed: %sn", nn_strerror(errno));
				continue;
			}

			int count = 0;
			while (1) {
				int bytes = this->recv();

				if (bytes < 0 && nn_errno() == ETIMEDOUT) break;

				if (bytes >= 0) {
					++count;
				}
				else {
					fprintf(stderr, "recv fail: %s. \n", nn_strerror(errno));
					break;
				}
			}
			printf("SERVER: current receive %d survey response. \n", count);
#ifdef _WIN32
			::Sleep(1000);
#else
			sleep(1);
#endif
		}

		this->shutdown();

		return 0;
	}

	virtual int onEnd() {
		return 0;
	}

public:
	virtual void onRecv(lw_int32 cmd, char* buf, lw_int32 bufsize) override {
		if (cmd == 1) {
			if (bufsize == sizeof(AddReponse)) {
				AddReponse* reponse = (AddReponse*)(buf);
				printf("size: %d, sum: %d\n", bufsize, reponse->c);
			}
		}
		else {

		}
	}
};

class SurveyClient : public NanomsgcppSocket, public Threadable
{
public:
	SurveyClient() {
	}

	virtual ~SurveyClient() {
	}

public:
	virtual int onStart() {
		return 0;
	}

	virtual int onRun() {

		const char *url = "tcp://127.0.0.1:8888";

		int sock = this->create(AF_SP, NN_RESPONDENT);
		if (sock < 0){
			fprintf(stderr, "nn_socket fail: %sn", nn_strerror(errno));
			return -1;
		}

		if (this->connect(url) < 0) {
			fprintf(stderr, "nn_connect fail: %sn", nn_strerror(errno));
			return -1;
		}

		while (1){
			int bytes = this->recv();
			if (bytes < 0) {
				fprintf(stderr, "recv fail: %s. \n", nn_strerror(errno));
			}
		}

		this->shutdown();

		return 0;
	}

	virtual int onEnd() {
		return 0;
	}

public:
	virtual void onRecv(lw_int32 cmd, char* buf, lw_int32 bufsize) override {
		if (cmd == 1) {
			if (bufsize == sizeof(AddRequest)) {
				AddRequest* request = (AddRequest*)(buf);
				printf("size: %d, a: %d, b: %d\n", bufsize, request->a, request->b);

				AddReponse reponse;
				reponse.c = request->a + request->b;
				int realSendSize = this->send(1, &reponse, sizeof(AddReponse));
			}
		}
		else {

		}
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

int client_main() {
	SurveyClient client;
	client.start();
	client.join();
	return 0;
}

int server_main() {
	SurveyServer server;
	server.start();
	server.join();

	return 0;
}

int main_nanomsg_survey(int argc, char** argv) {
	std::string s(argv[2]);

	if (s.compare("-s") == 0) {
		server_main();
	}
	else if (s.compare("-c") == 0) {
		client_main();
	}
	else {
		printf("please input survey -s or survey -c. \n");
	}

	return 0;
}
