#ifndef __DENMReceiver_H
#define __DENMReceiver_H

#include <PacketReceiver.hpp>
#include <DENMContainer.hpp>
#include <mutex>

class DENMReceiver : public PacketReceiver
{
protected:
	std::mutex msgContainerLock_;

	DENMReceiver();

	void decodeMessage(char* buffer, int len);

public:
	DENMContainer* msgContainer_;

	static DENMReceiver& getInstance()
	{
		static DENMReceiver instance;
		return instance;
	}

	void lockMsgContainer();
	void unlockMsgContainer();
};

#endif // __DENMReceiver_H