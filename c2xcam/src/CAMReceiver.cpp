#include <CAMReceiver.hpp>
#include <CAM.h>
#include <iostream>
#include <c2xcam.h>

namespace c2x {
CAMReceiver::CAMReceiver() {

}

void CAMReceiver::decodeMessage(char* buffer, int len) 
{
    int id, stationId;
	decodeCAM(&id, (uint8_t*)buffer, len);
    getCAMHeader(id, nullptr, nullptr, &stationId);

	std::cout << "Received message (length: " << len << " bytes)" << " from station " << stationId << " | ";
}
};