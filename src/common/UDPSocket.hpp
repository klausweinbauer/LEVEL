#pragma once

#include <ISocket.hpp>
#include <ISyscall.hpp>
#include <memory>
#include <string>

namespace level {

class UDPSocket : public ISocket {

private:
  std::shared_ptr<ISyscall> _sys;
  int _fd;
  SockAddrInet _addr;

  virtual void bindSocket();

public:
  UDPSocket(unsigned short port, std::shared_ptr<ISyscall> syscall);
  ~UDPSocket();

  virtual unsigned short port();

  virtual bool send(const char *buffer, int len) override;
  virtual int recv(char *buffer, int len, int timeout = 0) override;
  virtual bool read(char *buffer, int len, bool *cancel = nullptr) override;
};
}; // namespace level