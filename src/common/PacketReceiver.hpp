#pragma once

#include <Exception.hpp>
#include <ISocket.hpp>
#include <functional>
#include <thread>

namespace level {
class PacketReceiver {

private:
  bool _threadRunning = false;
  std::thread _recvThread;
  std::shared_ptr<ISocket> _socket;

  static void receive(PacketReceiver *receiver);

public:
  std::function<void(const char *, int)> recvPacketCallback;
  std::function<void(const Exception &)> errCallback;

  PacketReceiver(std::shared_ptr<ISocket> socket);
  virtual ~PacketReceiver();

  void start();

  PacketReceiver(PacketReceiver const &) = delete;
  void operator=(PacketReceiver const &) = delete;
};

}; // namespace level