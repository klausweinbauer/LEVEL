#include <PacketReceiver.hpp>

#include <chrono>
#include <iostream>
#include <thread>


namespace level {

PacketReceiver::PacketReceiver(unsigned short port) : _port(port), _socket() {
  _recvThread = std::thread(receive, this);
  while (!_threadRunning) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

PacketReceiver::~PacketReceiver() {
  _threadRunning = false;
  _socket.close();
  _recvThread.join();
}

void PacketReceiver::receive(PacketReceiver *receiver) {

  receiver->_socket.bindSocket(receiver->_port);

  int bufferSize = 65535;
  char *buffer = (char *)malloc(bufferSize);

  do {
    receiver->_threadRunning = true;

    try {
      sockaddr_in from_addr;
      int len = receiver->_socket.recvFrom(buffer, bufferSize, &from_addr);
      if (len <= 0) {
        continue;
      }

      if (receiver->recvPacketCallback) {
        receiver->recvPacketCallback(buffer, len);
      }

    } catch (const std::exception &e) {
      std::stringstream errMsg;
      errMsg << "Receiving message failed (" << e.what() << ")." << std::endl;
      Exception libException(ERR, errMsg.str().c_str());

      if (receiver->errCallback) {
        receiver->errCallback(libException);
      }

      std::this_thread::sleep_for(
          std::chrono::milliseconds(receiver->_errSleepTime));
    }
  } while (receiver->_threadRunning);

  free(buffer);
}

}; // namespace level