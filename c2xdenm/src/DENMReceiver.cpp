#include <DENMReceiver.hpp>
#include <DENMContainer.hpp>
#include <DENM.h>
#include <DENMService.hpp>
#include <iostream>

DENMReceiver::DENMReceiver() {

}

void DENMReceiver::decodeMessage(char* buffer, int len)
{
	DENM_t* newDENM = nullptr;
	decodeDENM(&newDENM, buffer, len);

	std::cout << "Received message (length: " << len << " bytes)" << " from station " << newDENM->header.stationID << " | ";

	DENMContainer::getInstance().lockContainer();
	DENMContainer::getInstance().addOrUpdate(newDENM);
	DENMContainer::getInstance().unlockContainer();
}