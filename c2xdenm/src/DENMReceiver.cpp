#include <DENMReceiver.hpp>
#include <DENMContainer.hpp>
#include <DENM.h>
#include <DENMService.hpp>
#include <iostream>

DENMReceiver::DENMReceiver()
	: msgContainer_(new DENMContainer())
{

}

void DENMReceiver::decodeMessage(char* buffer, int len)
{
	DENM_t* newDENM = nullptr;
	decodeDENM(&newDENM, buffer, len);

	std::cout << "Received message (length: " << len << " bytes)" << " from station " << newDENM->header.stationID << " | ";

	lockMsgContainer();
	msgContainer_->addOrUpdate(newDENM);
	unlockMsgContainer();
}

void DENMReceiver::lockMsgContainer()
{
	msgContainerLock_.lock();
}

void DENMReceiver::unlockMsgContainer()
{
	msgContainerLock_.unlock();
}