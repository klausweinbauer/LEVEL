#include <DENMReceiver.hpp>
#include <DENMContainer.hpp>
#include <DENM.h>
#include <iostream>
#include <c2xdenm.h>

namespace c2x {

DENMReceiver::DENMReceiver()
{

}

void DENMReceiver::decodeMessage(char* buffer, int len)
{
    int stationID, sequenceNumber;
	decodeDENM(&stationID, &sequenceNumber, (uint8_t*)buffer, len);

	std::cout << "Received message (length: " << len << " bytes)" << " from station " << stationID 
        << " | Sequence Number: " << sequenceNumber << std::endl;
}

};