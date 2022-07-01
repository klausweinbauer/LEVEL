#pragma once

#include <Exception.hpp>
#include <UDPSocket.hpp>
#include <functional>
#include <thread>

namespace c2x {
class PacketReceiver {

private:
  const int _errSleepTime = 10000;
  bool _threadRunning = false;
  unsigned short _port = 0;
  std::thread _recvThread;
  UDPSocket _socket;

  static void receive(PacketReceiver *receiver);

public:
  std::function<void(const char *, int)> recvPacketCallback;
  std::function<void(const Exception &)> errCallback;

  PacketReceiver(unsigned short port);
  virtual ~PacketReceiver();

  PacketReceiver(PacketReceiver const &) = delete;
  void operator=(PacketReceiver const &) = delete;
};

}; // namespace c2x