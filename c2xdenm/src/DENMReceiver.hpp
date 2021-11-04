#ifndef __DENMReceiver_H
#define __DENMReceiver_H

#include <PacketReceiver.hpp>

class DENMReceiver : public PacketReceiver
{
protected:
	DENMReceiver();

	void decodeMessage(char* buffer, int len);

public:
	static DENMReceiver& getInstance()
	{
		static DENMReceiver instance;
		return instance;
	}
};

#endif // __DENMReceiver_H