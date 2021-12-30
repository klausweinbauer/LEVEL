#include <DENMReceiver.hpp>
#include <DENM.h>
#include <iostream>
#include <c2xdenm.h>

namespace c2x {

DENMReceiver::DENMReceiver() : encoding_(XER_BASIC)
{

}

void DENMReceiver::decodeMessage(char* buffer, int len)
{
    int stationID, sequenceNumber;
	decodeDENM(&stationID, &sequenceNumber, (uint8_t*)buffer, len, encoding_);
    
    if (recvCallback != nullptr) {
        recvCallback(stationID, sequenceNumber);
    }
}

void DENMReceiver::setEncoding(EncodingType encoding)
{
    encoding_ = encoding;
}

EncodingType DENMReceiver::getEncoding()
{
    return encoding_;
}

};