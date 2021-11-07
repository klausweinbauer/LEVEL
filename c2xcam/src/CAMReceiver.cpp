#include <CAMReceiver.hpp>
#include <CAM.h>
#include <iostream>
#include <c2xcam.h>

namespace c2x {
CAMReceiver::CAMReceiver() {

}

void CAMReceiver::decodeMessage(char* buffer, int len) 
{
    int stationID;
	decodeCAM(&stationID, (uint8_t*)buffer, len);

	std::cout << "Received message (length: " << len << " bytes)" << " from station " << stationID << std::endl;
}

};