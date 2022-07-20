#include <PacketReceiver.hpp>

#ifndef WIN32
#include <unistd.h>
#endif

namespace level {

PacketReceiver::PacketReceiver(unsigned short port) : _port(port), _socket() {
  _recvThread = std::thread(receive, this);
  while (!_threadRunning) {
    usleep(1);
  }
}

PacketReceiver::~PacketReceiver() {
  _threadRunning = false;
  _socket.close();
  _recvThread.join();
}

void PacketReceiver::receive(PacketReceiver *receiver) {

#ifdef WIN32
  WSASession session;
#endif

  receiver->_threadRunning = true;
  receiver->_socket.bindSocket(receiver->_port);

  int bufferSize = 65535;
  char *buffer = (char *)malloc(bufferSize);

  while (receiver->_threadRunning) {

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

#ifdef WIN32
      Sleep(receiver->_errSleepTime);
#else
      usleep(receiver->_errSleepTime * 1000);
#endif
    }
  }

  free(buffer);
}

}; // namespace level