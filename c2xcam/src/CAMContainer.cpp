#include <CAMContainer.hpp>
#include <CAMService.hpp>
#include <iostream>

CAMContainer::CAMContainer() 
{

}

void CAMContainer::addOrUpdate(CAM_t* cam)
{
	if (get(cam->header.stationID))
	{
		remove(cam->header.stationID);
		std::cout << "CONTAINER -> Replace " << cam->header.stationID << std::endl;
	}
	else
	{
		std::cout << "CONTAINER -> New " << cam->header.stationID << std::endl;
	}

	msg_map_[cam->header.stationID] = cam;
}

void CAMContainer::remove(int stationId)
{
	CAM_t* cam = get(stationId);
	if (cam != nullptr)
	{
		freeCAMPointers(cam);
		free(cam);
		msg_map_.erase(stationId);
	}
}

CAM_t* CAMContainer::get(int stationId)
{
	if (msg_map_.count(stationId))
	{
		return msg_map_.at(stationId);
	}
	else
	{
		return nullptr;
	}
}

void CAMContainer::lockContainer()
{
	container_lock_.lock();
}

void CAMContainer::unlockContainer()
{
	container_lock_.unlock();
}

void CAMContainer::clear()
{
	for (auto it = msg_map_.begin(); it != msg_map_.end(); it++)
	{
		freeCAMPointers(it->second);
		free(it->second);
		it->second = nullptr;
	}

	msg_map_.clear();
}