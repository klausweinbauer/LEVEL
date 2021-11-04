#ifndef __DENMContainer_H
#define __DENMContainer_H

#include <DENM.h>
#include <vector>
#include <mutex>

typedef struct DENMEntry
{
	DENM_t* denm_;
	bool active_;
} DENMEntry_t;

class DENMContainer
{
private:
	std::vector<DENMEntry*> msg_list_;

public:
	DENMContainer();
	DENMContainer(DENMContainer const&) = delete;
	void operator=(DENMContainer const&) = delete;

	int addOrUpdate(DENM_t* DENM);
	int remove(ActionID_t actionId);
	int remove(unsigned long stationId, long sequenceNumber);
	DENM_t* get(ActionID_t actionId);
	DENM_t* get(unsigned long stationId, long sequenceNumber);
	int getIndex(ActionID_t actionId);
	int getIndex(unsigned long stationId, long sequenceNumber);
	DENM_t* getAtIndex(int index);
	int getActiveMessages(DENM_t** buffer, int len);
	int setActiveState(unsigned long stationID, long sequenceNumber, bool state);
	int clear();
};

#endif //__DENMContainer_H