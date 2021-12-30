#include <DENMTransmitter.hpp>
#include <iostream>
#include <UDPSocket.hpp>
#include <c2xdenm.h>

#ifndef WIN32
#include <unistd.h>
#endif

namespace c2x {

DENMTransmitter::DENMTransmitter() : encoding_(XER_BASIC)
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
    DENMTransmitter *instance = &DENMTransmitter::getInstance();

	char* buffer = (char*)malloc(TRANSMIT_BUFFER_LEN);
	while (instance->thread_running_)
	{
		try
		{
            int stationID, sequenceNumber, len;
            instance->src_lock_.lock();
            stationID = instance->src_stationID_;
            sequenceNumber = instance->src_sequenceNumber_;
            instance->src_lock_.unlock();

            if (instance->sendCallback != nullptr) {
                instance->sendCallback(stationID, sequenceNumber);
            }
            int encRet = c2x::encodeDENM(stationID, sequenceNumber, (uint8_t*)buffer, TRANSMIT_BUFFER_LEN, &len, instance->encoding_);
            
			if (encRet > 0) 
            {
                socket.sendTo(instance->port_, buffer, len);
            }

#ifdef WIN32
			Sleep(instance->interval_ms_);
#else
			usleep(instance->interval_ms_ * 1000);
#endif
		}
		catch (const std::exception& ex)
		{
			std::cout << "DENMTransmitter::send() | " << ex.what() << std::endl;

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

void DENMTransmitter::setMessageSource(int stationID, int sequenceNumber)
{
    src_lock_.lock();
    src_stationID_ = stationID;
    src_sequenceNumber_ = sequenceNumber;
    src_lock_.unlock();
}

void DENMTransmitter::setEncoding(EncodingType encoding)
{
    encoding_ = encoding;
}

EncodingType DENMTransmitter::getEncoding()
{
    return encoding_;
}

};