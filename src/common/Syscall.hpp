// Copyright 2022 Klaus Weinbauer
//
// This file is part of LEVEL.
//
// LEVEL is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// LEVEL is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LEVEL. If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <ISyscall.hpp>

namespace level {

class Syscall : public ISyscall {

public:
  virtual ~Syscall();

  virtual int sysPoll(PollFD *fds, nfds_l nfds, int timeout);

  virtual int sysSocket(SockDomain domain, SockType type, Protocol protocol);

  virtual int sysSendTo(int sockfd, const void *buf, size_t len, int flags,
                        const SockAddr *dest_addr, SockLen addrlen);

  virtual int sysSetSockOpt(int socket, ProtocolLevel level,
                            SocketOption option_name,
                            const void *option_value = nullptr,
                            SockLen option_len = 0);

  virtual int sysClose(int fd);

  virtual int sysBind(int sockfd, const SockAddr *addr, SockLen addrlen);

  virtual int sysRecvFrom(int sockfd, void *buf, size_t len, int flags,
                          SockAddr *src_addr = nullptr,
                          SockLen *addrlen = nullptr);
};

} // namespace level