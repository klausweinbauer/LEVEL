#ifndef __PacketReceiver_H
#define __PacketReceiver_H

#include <thread>
#include <UDPSocket.hpp>

#define RECEIVE_BUFFER_LEN 65535

class PacketReceiver
{
private:
	bool thread_running_ = false;
	unsigned short port_ = 0;
	std::thread receiving_thread_;

	static void receive(void* obj);

protected:
	PacketReceiver();
	virtual void decodeMessage(char* buffer, int len) = 0;

public:
	virtual ~PacketReceiver();
	PacketReceiver(PacketReceiver const&) = delete;
	void operator=(PacketReceiver const&) = delete;

	void start(unsigned short port = 1997);
	void stop();
};

#endif //__PacketReceiver_H