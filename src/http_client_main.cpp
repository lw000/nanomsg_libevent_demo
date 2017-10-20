#include "main.h"
#include <stdio.h>
#include "utils.h"
#include <thread>
#include "http_client.h"

#include "log4z.h"

static void __run_http_client() {

}

int main_http_client(int argc, char** argv)
{
	char hosts[][256] = {
		"http://127.0.0.1:9877/add?a=111&b=2222",
	};

	HttpClient client;
	bool r = client.create();
	if (r) {
		client.start();

		while(1)
		{
			getchar();

			for (int i = 0; i < 100; i++) {
				HttpRequest* request = new HttpRequest(&client);
				request->setTag(i);
				request->setUrl(hosts[0]);
				request->setMethod(HttpRequest::MEDHOD::GET);
				request->setCallback([](HttpReponse* reponse) {
					std::string data = reponse->getContent();
					LOGFMTD("tag: %d, %s", reponse->getTag(), data.c_str());
				});
				client.add(request);
				lw_sleep(10);
			}
		}
		
	}

	while (1)
	{
		lw_sleep(10);
	}

	return 0;
}

