#include <DENMContainer.hpp>
#include <DENMService.hpp>
#include <iostream>

DENMContainer::DENMContainer()
{

}

int DENMContainer::addOrUpdate(DENM_t* denm)
{
	lockContainer();

	DENM_t* b_denm = get(denm->denm.management.actionID);
	if (b_denm)
	{
		if (b_denm->header.stationID != denm->header.stationID)
		{
			unlockContainer();
			return -1;
		}

		int index = getIndex(denm->denm.management.actionID);
		msg_list_[index] = denm;
		freeDENM(b_denm);

		std::cout << "DENM-CONTAINER -> Replace [" << index << "] "
			<< denm->denm.management.actionID.originatingStationID << "-"
			<< denm->denm.management.actionID.sequenceNumber << std::endl;
	}
	else
	{
		msg_list_.insert(msg_list_.begin(), denm);
		std::cout << "DENM-CONTAINER -> New "
			<< denm->denm.management.actionID.originatingStationID << "-"
			<< denm->denm.management.actionID.sequenceNumber << std::endl;
	}

	unlockContainer();
}

int DENMContainer::remove(int stationId, int sequenceNumber)
{
	lockContainer();

	int index = getIndex(stationId, sequenceNumber);
	if (index >= 0)
	{
		DENM_t* denm = msg_list_[index];
		msg_list_.erase(msg_list_.begin() + index);
		//freeDENMPointers(denm);
		free(denm);
	}

	unlockContainer();

	return 0;
}

DENM_t* DENMContainer::get(int stationId, int sequenceNumber)
{
	for (auto it = msg_list_.begin(); it != msg_list_.end(); it++)
	{
		if ((*it)->denm.management.actionID.originatingStationID == stationId
			&& (*it)->denm.management.actionID.sequenceNumber == sequenceNumber)
		{
			return *it;
		}
	}

	return nullptr;
}

void DENMContainer::lockContainer()
{
	container_lock_.lock();
}

void DENMContainer::unlockContainer()
{
	container_lock_.unlock();
}

int DENMContainer::clear()
{
	for (auto it = msg_list_.begin(); it != msg_list_.end(); it++)
	{
		//freeDENMPointers(it->second);
		free(*it);
	}

	msg_list_.clear();

	return 0;
}

int DENMContainer::getIndex(int stationId, int sequencNumber)
{
	for (int i = 0; i < msg_list_.size(); i++)
	{
		if (msg_list_[i]->denm.management.actionID.originatingStationID == stationId
			&& msg_list_[i]->denm.management.actionID.sequenceNumber == sequencNumber)
		{
			return i;
		}
	}

	return -1;
}

DENM_t* DENMContainer::getAtIndex(int index)
{
	if (index < msg_list_.size())
	{
		return msg_list_[index];
	}
	else
	{
		return nullptr;
	}
}

int DENMContainer::remove(ActionID_t actionId)
{
	return remove(actionId.originatingStationID, actionId.sequenceNumber);
}

DENM_t* DENMContainer::get(ActionID_t actionId)
{
	return get(actionId.originatingStationID, actionId.sequenceNumber);
}

int DENMContainer::getIndex(ActionID_t actionId)
{
	return getIndex(actionId.originatingStationID, actionId.sequenceNumber);
}

int DENMContainer::getActiveMessages(int stationId, DENM_t** buffer, int len)
{
	int outIndex = 0;
	for (int i = 0; i < msg_list_.size(); i++)
	{
		if (outIndex == len) {
			return outIndex;
		}

		if (msg_list_[i]->denm.management.actionID.originatingStationID == stationId)
		{
			buffer[outIndex] = msg_list_[i];
			outIndex++;
		}
	}

	return outIndex;
}