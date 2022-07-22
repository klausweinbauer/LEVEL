#include <PacketReceiver.hpp>

#include <chrono>
#include <iostream>
#include <thread>

namespace level {

PacketReceiver::PacketReceiver(std::shared_ptr<ISocket> socket)
    : _socket(socket) {}

PacketReceiver::~PacketReceiver() {
  if (_threadRunning) {
    _threadRunning = false;
    _socket->close();
    _recvThread.join();
  }
}

void PacketReceiver::start() {
  _recvThread = std::thread(receive, this);
  while (!_threadRunning) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void PacketReceiver::receive(PacketReceiver *receiver) {

  int bufferSize = 65535;
  char *buffer = (char *)malloc(bufferSize);

  do {
    receiver->_threadRunning = true;

    try {
      int len = receiver->_socket->recv(buffer, bufferSize);
      if (len <= 0) {
        continue;
      }

      if (receiver->recvPacketCallback) {
        receiver->recvPacketCallback(buffer, len);
      }

    } catch (const Exception &e) {
      if (receiver->errCallback) {
        receiver->errCallback(e);
      }
    }
  } while (receiver->_threadRunning);

  free(buffer);
}

}; // namespace level