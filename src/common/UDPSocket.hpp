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

#include <ISocket.hpp>
#include <ISyscall.hpp>
#include <memory>
#include <string>

// Though UDP can handle up to 65535kB (- header size) data packets, this is not
// practical. So we split data into chunks of SOCKET_MAX_SEND_SIZE.
#ifndef SOCKET_MAX_SEND_SIZE
#define SOCKET_MAX_SEND_SIZE 4096
#endif

// The default timeout used in read.
#ifndef SOCKET_READ_TIMOUT
#define SOCKET_READ_TIMOUT 500
#endif

namespace level {

class UDPSocket : public ISocket {

private:
  std::shared_ptr<ISyscall> _sys;
  int _fd;
  unsigned short _port;
  bool _isBound;

  virtual void bindSocket();
  virtual int handleRecv(char *buffer, int len);

public:
  UDPSocket(unsigned short port, std::shared_ptr<ISyscall> syscall);
  ~UDPSocket();

  virtual unsigned short port();

  virtual bool send(const char *buffer, int len) override;
  virtual int recv(char *buffer, int len, int timeout = 0) override;
  virtual int read(char *buffer, int len,
                   const bool *const cancel = nullptr) override;
};
}; // namespace level