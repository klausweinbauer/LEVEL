#ifndef __DENMTransmitter_H
#define __DENMTransmitter_H

#include <DENM.h>
#include <thread>
#include <mutex>

#define TRANSMIT_BUFFER_LEN 65535

class DENMTransmitter
{
private:
	unsigned short port_ = 0;
	bool thread_running_ = false;
	unsigned int interval_ms_ = 1000;
	std::thread send_thread_;
	int stationId_;

	DENMTransmitter();

	static void send();

public:
	static DENMTransmitter& getInstance()
	{
		static DENMTransmitter instance;
		return instance;
	}

	DENMTransmitter(DENMTransmitter const&) = delete;
	void operator=(DENMTransmitter const&) = delete;

	void start(int stationId, unsigned short port, unsigned int interval_ms);
	void stop();
	void setInterval(unsigned int interval);
	unsigned int getInterval();
};

#endif //__DENMTransmitter_H