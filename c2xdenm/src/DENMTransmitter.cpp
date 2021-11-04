#include <DENMTransmitter.hpp>
#include <DENMService.hpp>
#include <DENMContainer.hpp>
#include <iostream>
#include <UDPSocket.hpp>

#ifndef WIN32
#include <unistd.h>
#endif

DENMTransmitter::DENMTransmitter()
	: msgContainer_(new DENMContainer())
{

}

void DENMTransmitter::start(unsigned short port, unsigned int interval_ms)
{
	if (thread_running_)
	{
		return;
	}

	interval_ms_ = interval_ms;
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
	std::cout << "DENM Transmitter started." << std::endl;

#ifdef WIN32
	WSASession session;
#endif
	UDPSocket socket;

	char* buffer = (char*)malloc(TRANSMIT_BUFFER_LEN);
	while (DENMTransmitter::getInstance().thread_running_)
	{
		try
		{
			DENMTransmitter::getInstance().lockMsgContainer();
			DENM_t* active_msgs[100];
			int active_msgs_len = DENMTransmitter::getInstance().msgContainer_->getActiveMessages(active_msgs, 100);
			for (int i = 0; i < active_msgs_len; i++)
			{
				int len = encodeDENM(active_msgs[i], buffer, TRANSMIT_BUFFER_LEN);
				std::cout << "Send DENM message (length: " << len << " bytes) from station " 
					<< active_msgs[i]->denm.management.actionID.originatingStationID << " - "
					<< active_msgs[i]->denm.management.actionID.sequenceNumber << std::endl;
				socket.sendTo(DENMTransmitter::getInstance().port_, buffer, len);
			}			
			DENMTransmitter::getInstance().unlockMsgContainer();

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
	std::cout << "DENM Transmitter stopped." << std::endl;
}

void DENMTransmitter::setInterval(unsigned int interval)
{
	interval_ms_ = interval;
}

unsigned int DENMTransmitter::getInterval()
{
	return interval_ms_;
}

void DENMTransmitter::lockMsgContainer()
{
	msgContainerLock_.lock();
}

void DENMTransmitter::unlockMsgContainer()
{
	msgContainerLock_.unlock();
}