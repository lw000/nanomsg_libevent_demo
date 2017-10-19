// main.cpp :
//

#include "main.h"

#include "log4z.h"
#include "Threadable.h"
#include "socket_processor.h"
#include "utils.h"

class TimerTest : public Threadable {
	SocketProcessor* _processor;
public:
	TimerTest(SocketProcessor* processor) : _processor(processor) {}
	virtual ~TimerTest() {}

public:
	virtual int onStart() {
		return 0;
	}

	virtual int onRun() {

		for (int i = 0; i < 50000; i++) {
			this->_processor->addTimer(i, 1000, [](int tid, int tms) -> bool {
				LOGFMTD("tid: %d", tid);
				return true;
			});
		}

		return 0;
	}

	virtual int onEnd() {

		return 0;
	}
};

int main_timer_server(int argc, char** argv)
{
	SocketProcessor* processor = new SocketProcessor;
	processor->create(true);

	(new TimerTest(processor))->start();

	lw_sleep(1000);

	int c = processor->dispatch();
	LOGFMTD("c = %d", c);

	return 0;
}

