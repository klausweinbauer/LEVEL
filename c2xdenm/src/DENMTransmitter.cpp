#include <DENMTransmitter.hpp>
#include <iostream>
#include <UDPSocket.hpp>

#ifndef WIN32
#include <unistd.h>
#endif

namespace c2x {

DENMTransmitter::DENMTransmitter()
{

}

void DENMTransmitter::start(unsigned short port)
{
	if (thread_running_)
	{
		return;
	}

	port_ = port;

	thread_running_ = true;
	send_thread_ = std::thread(send);
}

void DENMTransmitter::stop()
{
	if (!thread_running_)
	{
		return;
	}

	thread_running_ = false;
	send_thread_.join();
}

void DENMTransmitter::send()
{
#ifdef WIN32
	WSASession session;
#endif
	UDPSocket socket;

	char* buffer = (char*)malloc(TRANSMIT_BUFFER_LEN);
	while (DENMTransmitter::getInstance().thread_running_)
	{
		try
		{
			int len = 0;
			socket.sendTo(DENMTransmitter::getInstance().port_, buffer, len);

#ifdef WIN32
			Sleep(DENMTransmitter::getInstance().interval_ms_);
#else
			usleep(DENMTransmitter::getInstance().interval_ms_ * 1000);
#endif
		}
		catch (const std::exception& ex)
		{
			std::cout << "[Error] DENMTransmitter::send() | " << ex.what() << std::endl;

			const int wait_time = 10000;
#ifdef WIN32
			Sleep(wait_time);
#else
			usleep(wait_time * 1000);
#endif
		}
	}
	free(buffer);
}

void DENMTransmitter::setInterval(unsigned int interval)
{
	interval_ms_ = interval;
}

unsigned int DENMTransmitter::getInterval()
{
	return interval_ms_;
}
};