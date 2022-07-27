#pragma once

#include <ISyscall.hpp>

namespace level {

class Syscall : public ISyscall {

public:
  virtual ~Syscall();

  virtual int sysPoll(PollFD *fds, nfds_l nfds, int timeout);

  virtual int sysSocket(SockDomain domain, SockType type, Protocol protocol);

  virtual ssize_t sysSendTo(int sockfd, const void *buf, size_t len, int flags,
                            const SockAddr *dest_addr, SockLen addrlen);

  virtual int sysSetSockOpt(int socket, ProtocolLevel level,
                            SocketOption option_name,
                            const void *option_value = nullptr,
                            SockLen option_len = 0);

  virtual int sysClose(int fd);

  virtual int sysBind(int sockfd, const SockAddr *addr, SockLen addrlen);

  virtual ssize_t sysRecvFrom(int sockfd, void *buf, size_t len, int flags,
                              SockAddr *src_addr = nullptr,
                              SockLen *addrlen = nullptr);
};

} // namespace level