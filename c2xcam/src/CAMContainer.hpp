#ifndef __CAMContainer_H
#define __CAMContainer_H

#include <CAM.h>
#include <map>
#include <mutex>

class CAMContainer
{
private:	
	std::map<long, CAM_t*> msg_map_;
	std::mutex container_lock_;

	CAMContainer();

public:
	static CAMContainer& getInstance()
	{
		static CAMContainer instance;
		return instance;
	}

	CAMContainer(CAMContainer const&) = delete;
	void operator=(CAMContainer const&) = delete;

	void lockContainer();
	void unlockContainer();
	void addOrUpdate(CAM_t* cam);
	void remove(int stationId);
	CAM_t* get(int stationId);
	void clear();
};

#endif //__CAMContainer_H