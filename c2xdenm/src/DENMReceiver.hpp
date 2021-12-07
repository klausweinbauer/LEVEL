#pragma once

#include <PacketReceiver.hpp>
#include <mutex>

namespace c2x {

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

};