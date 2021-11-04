#ifndef __CAMReceiver_H
#define __CAMReceiver_H

#include <PacketReceiver.hpp>

class CAMReceiver : public PacketReceiver
{
protected:
	CAMReceiver();

	void decodeMessage(char* buffer, int len);

public:
	static CAMReceiver& getInstance()
	{
		static CAMReceiver instance;
		return instance;
	}
};

#endif // __CAMReceiver_H