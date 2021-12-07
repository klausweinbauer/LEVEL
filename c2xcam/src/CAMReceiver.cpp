#include <CAMReceiver.hpp>
#include <CAM.h>
#include <iostream>
#include <c2xcam.h>

namespace c2x {
CAMReceiver::CAMReceiver() {

}

void CAMReceiver::decodeMessage(char* buffer, int len) 
{
	last_error_ = 0;
    int stationID;
	last_error_ = decodeCAM(&stationID, (uint8_t*)buffer, len);
}

int CAMReceiver::getLastError()
{
	return last_error_;
}

};