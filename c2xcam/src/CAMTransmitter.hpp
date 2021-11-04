#ifndef __CAMTransmitter_H
#define __CAMTransmitter_H

#include <CAM.h>
#include <thread>
#include <mutex>

#define TRANSMIT_BUFFER_LEN 65535

class CAMTransmitter
{
private:
	unsigned short port_ = 0;
	bool thread_running_ = false;
	unsigned int interval_ms_ = 1000;
	std::thread send_thread_;
	int stationId_ = -1;
	CAM_t cam_;
	std::mutex cam_lock_;

	CAMTransmitter();

	static void send();

public:
	static CAMTransmitter& getInstance()
	{
		static CAMTransmitter instance;
		return instance;
	}

	CAMTransmitter(CAMTransmitter const&) = delete;
	void operator=(CAMTransmitter const&) = delete;

	void start(unsigned short port = 1997, unsigned int interval_ms = 1000);
	void stop();
	void setInterval(unsigned int interval);
	void setStationId(int stationId);
	unsigned int getInterval();
	CAM_t* localCAM();
	void lockCAM();
	void unlockCAM();
};

#endif //__CAMTransmitter_H