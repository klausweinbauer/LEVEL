#pragma once

#include <ISocket.hpp>
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

class UDPSocket : public ISocket {

private:
  unsigned short _port;
  bool _enableRecvException;
  bool _isBound;

#ifdef _WIN32
  SOCKET _sock;
  WSASession _session;
#else
  int _sock;
#endif

public:
  UDPSocket(unsigned short port);
  ~UDPSocket();

  virtual void send(const char *buffer, int len, int flags = 0) override;
  virtual int recv(char *buffer, int len, int flags = 0) override;
  virtual void bindSocket();
  virtual void close() override;
};
}; // namespace level