#pragma once

#include <ISocket.hpp>
#include <ISyscall.hpp>
#include <memory>
#include <string>


namespace level {

class UDPSocket : public ISocket {

private:
  unsigned short _port;
  std::shared_ptr<ISyscall> _sys;
  int _fd;

public:
  UDPSocket(unsigned short port, std::shared_ptr<ISyscall> syscall);
  ~UDPSocket();

  virtual void send(const char *buffer, int len) override;
  virtual int recv(char *buffer, int len) override;
  virtual void bindSocket();
  virtual void close() override;
};
}; // namespace level