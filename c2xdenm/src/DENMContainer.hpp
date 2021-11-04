#ifndef __DENMContainer_H
#define __DENMContainer_H

#include <DENM.h>
#include <vector>
#include <mutex>

class DENMContainer
{
private:
	std::vector<DENM_t*> msg_list_;
	std::mutex container_lock_;

	DENMContainer();

public:
	static DENMContainer& getInstance()
	{
		static DENMContainer instance;
		return instance;
	}

	DENMContainer(DENMContainer const&) = delete;
	void operator=(DENMContainer const&) = delete;

	void lockContainer();
	void unlockContainer();
	int addOrUpdate(DENM_t* DENM);
	int remove(ActionID_t actionId);
	int remove(int stationId, int sequenceNumber);
	DENM_t* get(ActionID_t actionId);
	DENM_t* get(int stationId, int sequenceNumber);
	int getIndex(ActionID_t actionId);
	int getIndex(int stationId, int sequenceNumber);
	DENM_t* getAtIndex(int index);
	int getActiveMessages(int stationId, DENM_t** buffer, int len);
	int clear();
};

#endif //__DENMContainer_H