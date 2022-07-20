#pragma once

#include <string>

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <netinet/in.h>
#endif

namespace level {

#ifdef _WIN32
class WSASession {
public:
  WSASession();
  ~WSASession();

private:
  WSAData data_;
};
#endif

class UDPSocket {
private:
  bool _recvFail;

#ifdef _WIN32
  SOCKET _sock;
#else
  int _sock;
#endif

public:
  UDPSocket();
  ~UDPSocket();

  void sendTo(unsigned short port, const char *buffer, int len, int flags = 0);
  int recvFrom(char *buffer, int len, sockaddr_in *from_addr, int flags = 0);
  void bindSocket(unsigned short port);
  void close();
};
}; // namespace level