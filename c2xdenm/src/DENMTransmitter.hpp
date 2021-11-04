#ifndef __DENMTransmitter_H
#define __DENMTransmitter_H

#include <DENM.h>
#include <thread>
#include <mutex>
#include <DENMContainer.hpp>

#define TRANSMIT_BUFFER_LEN 65535

class DENMTransmitter
{
private:
	unsigned short port_ = 0;
	bool thread_running_ = false;
	unsigned int interval_ms_ = 1000;
	std::thread send_thread_;
	std::mutex msgContainerLock_;

	DENMTransmitter();

	static void send();

public:
	DENMContainer* msgContainer_;

	static DENMTransmitter& getInstance()
	{
		static DENMTransmitter instance;
		return instance;
	}

	DENMTransmitter(DENMTransmitter const&) = delete;
	void operator=(DENMTransmitter const&) = delete;

	void start(unsigned short port, unsigned int interval_ms);
	void stop();
	void setInterval(unsigned int interval);
	unsigned int getInterval();
	void lockMsgContainer();
	void unlockMsgContainer();
};

#endif //__DENMTransmitter_H