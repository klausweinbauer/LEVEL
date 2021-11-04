#include <CAMReceiver.hpp>
#include <CAMContainer.hpp>
#include <CAM.h>
#include <CAMService.hpp>
#include <iostream>

CAMReceiver::CAMReceiver() {

}

void CAMReceiver::decodeMessage(char* buffer, int len) 
{
	CAM_t* newCAM = nullptr;
	decodeCAM(&newCAM, buffer, len);

	std::cout << "Received message (length: " << len << " bytes)" << " from station " << newCAM->header.stationID << " | ";

	CAMContainer::getInstance().lockContainer();
	CAMContainer::getInstance().addOrUpdate(newCAM);
	CAMContainer::getInstance().unlockContainer();
}