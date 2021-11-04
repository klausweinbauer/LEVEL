#include <DENMContainer.hpp>
#include <DENMService.hpp>
#include <iostream>

DENMContainer::DENMContainer()
{

}

int DENMContainer::addOrUpdate(DENM_t* denm)
{
	DENM_t* b_denm = get(denm->denm.management.actionID);
	if (b_denm)
	{
		if (b_denm->header.stationID != denm->header.stationID)
		{
			return -1;
		}

		int index = getIndex(denm->denm.management.actionID);
		msg_list_[index]->denm_ = denm;
		freeDENM(b_denm);

		std::cout << "DENM-CONTAINER -> Replace [" << index << "] "
			<< denm->denm.management.actionID.originatingStationID << "-"
			<< denm->denm.management.actionID.sequenceNumber << std::endl;
	}
	else
	{
		DENMEntry* denmEntry = new DENMEntry();
		denmEntry->active_ = 0;
		denmEntry->denm_ = denm;
		msg_list_.insert(msg_list_.begin(), denmEntry);
		std::cout << "DENM-CONTAINER -> New "
			<< denm->denm.management.actionID.originatingStationID << "-"
			<< denm->denm.management.actionID.sequenceNumber << std::endl;
	}
}

int DENMContainer::remove(unsigned long stationId, long sequenceNumber)
{
	int index = getIndex(stationId, sequenceNumber);
	if (index >= 0)
	{
		DENM_t* denm = msg_list_[index]->denm_;
		msg_list_.erase(msg_list_.begin() + index);
		freeDENM(denm);
	}

	return 0;
}

DENM_t* DENMContainer::get(unsigned long stationId, long sequenceNumber)
{
	for (auto it = msg_list_.begin(); it != msg_list_.end(); it++)
	{
		if ((*it)->denm_->denm.management.actionID.originatingStationID == stationId
			&& (*it)->denm_->denm.management.actionID.sequenceNumber == sequenceNumber)
		{
			return (*it)->denm_;
		}
	}

	return nullptr;
}

int DENMContainer::clear()
{
	for (auto it = msg_list_.begin(); it != msg_list_.end(); it++)
	{
		freeDENM((*it)->denm_);
		delete *it;
	}

	msg_list_.clear();

	return 0;
}

int DENMContainer::getIndex(unsigned long stationId, long sequencNumber)
{
	for (int i = 0; i < msg_list_.size(); i++)
	{
		if (msg_list_[i]->denm_->denm.management.actionID.originatingStationID == stationId
			&& msg_list_[i]->denm_->denm.management.actionID.sequenceNumber == sequencNumber)
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
		return msg_list_[index]->denm_;
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

int DENMContainer::getActiveMessages(DENM_t** buffer, int len)
{
	int outIndex = 0;
	for (int i = 0; i < msg_list_.size(); i++)
	{
		if (outIndex == len) {
			return outIndex;
		}

		if (!msg_list_[i]->active_)
		{
			continue;
		}

		buffer[outIndex] = msg_list_[i]->denm_;
		outIndex++;
	}

	return outIndex;
}

int DENMContainer::setActiveState(unsigned long stationID, long sequenceNumber, bool state)
{
	int index = getIndex(stationID, sequenceNumber);
	if (index < 0)
	{
		return -1;
	}

	msg_list_[index]->active_ = state;
}