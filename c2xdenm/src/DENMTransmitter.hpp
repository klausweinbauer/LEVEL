#pragma once

#include <DENM.h>
#include <thread>
#include <mutex>

#define TRANSMIT_BUFFER_LEN 65535

namespace c2x {

class DENMTransmitter
{
private:
	unsigned short port_ = 0;
	bool thread_running_ = false;
	unsigned int interval_ms_ = 1000;
	std::thread send_thread_;
    int src_stationID_ = 0;
    int src_sequenceNumber_ = 0;
    std::mutex src_lock_;

	DENMTransmitter();

	static void send();

public:
	static DENMTransmitter& getInstance()
	{
		static DENMTransmitter instance;
		return instance;
	}

    void (*sendCallback)(int stationId, int sequenceNumber);

	DENMTransmitter(DENMTransmitter const&) = delete;
	void operator=(DENMTransmitter const&) = delete;

	void start(unsigned short port);
	void stop();
	void setInterval(unsigned int interval_ms);
    void setMessageSource(int stationID, int sequenceNumber);
	unsigned int getInterval();
};
};