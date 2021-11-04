#include <CAMTransmitter.hpp>
#include <CAMService.hpp>
#include <iostream>
#include <UDPSocket.hpp>

#ifndef WIN32
#include <unistd.h>
#endif

CAMTransmitter::CAMTransmitter() : cam_()
{
	
}

void CAMTransmitter::start(unsigned short port, unsigned int interval_ms)
{
	if (thread_running_)
	{
		//throw std::invalid_argument("Packet transmitter already started.");
		return;
	}

	interval_ms_ = interval_ms;
	port_ = port;

	thread_running_ = true;
	send_thread_ = std::thread(send);
}

void CAMTransmitter::stop()
{
	if (!thread_running_)
	{
		//throw std::invalid_argument("Transmitting thread is not running.");
		return;
	}

	thread_running_ = false;
	send_thread_.join();
}

void CAMTransmitter::send()
{
	std::cout << "Transmitting thread started." << std::endl;

#ifdef WIN32
	WSASession session;
#endif
	UDPSocket socket;

	char* buffer = (char*)malloc(TRANSMIT_BUFFER_LEN);
	while (CAMTransmitter::getInstance().thread_running_)
	{
		try
		{
			CAMTransmitter::getInstance().lockCAM();
			int stationId = CAMTransmitter::getInstance().cam_.header.stationID;
			int len = encodeCAM(CAMTransmitter::getInstance().localCAM(), buffer, TRANSMIT_BUFFER_LEN);
			CAMTransmitter::getInstance().unlockCAM();

			std::cout << "Send CAM message (length: " << len << " bytes) from station " << stationId << std::endl;
			socket.sendTo(CAMTransmitter::getInstance().port_, buffer, len);

            #ifdef WIN32
			Sleep(CAMTransmitter::getInstance().interval_ms_);
            #else
            usleep(CAMTransmitter::getInstance().interval_ms_ * 1000);
            #endif
		}
		catch (const std::exception& ex)
		{
			std::cout << "[Error] CAMTransmitter::send() | " << ex.what() << std::endl;

            const int wait_time = 10000;
            #ifdef WIN32
			Sleep(wait_time);
            #else
            usleep(wait_time * 1000);
            #endif
		}
	}
	free(buffer);
	std::cout << "Transmitting thread stopped." << std::endl;
}

void CAMTransmitter::setInterval(unsigned int interval)
{
	interval_ms_ = interval;
}

unsigned int CAMTransmitter::getInterval()
{
	return interval_ms_;
}

void CAMTransmitter::setStationId(int stationId)
{
	stationId_ = stationId;
}

CAM_t* CAMTransmitter::localCAM()
{
	return &cam_;
}

void CAMTransmitter::lockCAM()
{
	cam_lock_.lock();
}

void CAMTransmitter::unlockCAM()
{
	cam_lock_.unlock();
}