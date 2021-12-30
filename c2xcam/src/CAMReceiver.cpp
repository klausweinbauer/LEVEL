#include <CAMReceiver.hpp>
#include <CAM.h>
#include <iostream>
#include <c2xcam.h>

namespace c2x {
CAMReceiver::CAMReceiver() : encoding_(XER_BASIC) {

}

void CAMReceiver::decodeMessage(char* buffer, int len) 
{
	last_error_ = 0;
    int stationID;
	last_error_ = decodeCAM(&stationID, (uint8_t*)buffer, len, encoding_);

    if (recvCallback != nullptr) {
        recvCallback(stationID);
    }
}

int CAMReceiver::getLastError()
{
	return last_error_;
}

EncodingType CAMReceiver::getEncoding()
{
    return encoding_;
}

void CAMReceiver::setEncoding(EncodingType encoding)
{
    encoding_ = encoding;
}

};