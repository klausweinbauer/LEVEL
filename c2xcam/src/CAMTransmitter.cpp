#include <CAMTransmitter.hpp>
#include <CAMService.hpp>
#include <iostream>
#include <UDPSocket.hpp>
#include <c2xcommon.h>
#include <c2xcam.h>

#ifndef WIN32
#include <unistd.h>
#endif

namespace c2x {

CAMTransmitter::CAMTransmitter()
{
	
}

void CAMTransmitter::start(unsigned short port)
{
	if (thread_running_)
	{
		return;
	}

	port_ = port;

	thread_running_ = true;
	send_thread_ = std::thread(send);
}

void CAMTransmitter::stop()
{
	if (!thread_running_)
	{
		return;
	}

	thread_running_ = false;
	send_thread_.join();

    transmit_ids_lock_.lock();
    free(transmit_ids_);
    transmit_ids_lock_.unlock();
}

void CAMTransmitter::send()
{
#ifdef WIN32
	WSASession session;
#endif
	UDPSocket socket;

	char* buffer = (char*)malloc(TRANSMIT_BUFFER_LEN);
	while (CAMTransmitter::getInstance().thread_running_)
	{
		try
		{
            CAMTransmitter::getInstance().transmit_ids_lock_.lock();
            for (int i = 0; i < CAMTransmitter::getInstance().transmit_ids_size_; i++) {
                int len = 0;
                int stationID = CAMTransmitter::getInstance().transmit_ids_[i];
                c2x::encodeCAM(stationID, (uint8_t*)buffer, TRANSMIT_BUFFER_LEN, &len);
                std::cout << "Send CAM message (length: " << len << " bytes) from station " << stationID << std::endl;
                socket.sendTo(CAMTransmitter::getInstance().port_, buffer, len);
            }
            CAMTransmitter::getInstance().transmit_ids_lock_.unlock();

            #ifdef WIN32
			Sleep(CAMTransmitter::getInstance().interval_ms_);
            #else
            usleep(CAMTransmitter::getInstance().interval_ms_ * 1000);
            #endif
		}
		catch (const std::exception& ex)
		{
			std::cout << "CAMTransmitter::send() | " << ex.what() << std::endl;

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

void CAMTransmitter::setInterval(unsigned int interval_ms)
{
	interval_ms_ = interval_ms;
}

unsigned int CAMTransmitter::getInterval()
{
	return interval_ms_;
}

int CAMTransmitter::setIDsToTransmit(int *ids, int size)
{
    if (!ids) {
        return ERR_ARG_NULL;
    }

    transmit_ids_lock_.lock();
    if (!transmit_ids_) {
        transmit_ids_ = (int*)malloc(size * sizeof(int));
    }
    else {
        transmit_ids_ = (int*)realloc(transmit_ids_, size * sizeof(int));
    }
    transmit_ids_size_ = 0;

    if (!transmit_ids_) {
        transmit_ids_lock_.unlock();
        return ERR_ALLOC_FAILED;
    }

    memcpy(transmit_ids_, ids, size * sizeof(int));
    transmit_ids_size_ = size;

    transmit_ids_lock_.unlock();
    return 0;
}

};