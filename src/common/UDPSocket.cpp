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

#include <NetworkException.hpp>
#include <UDPSocket.hpp>
#include <cassert>
#include <memory>
#include <sstream>
#include <string.h>

namespace level {

UDPSocket::UDPSocket(unsigned short port, std::shared_ptr<ISyscall> syscall)
    : _sys(syscall), _fd(0), _port(port), _isBound(false) {
  if (!_sys) {
    throw Exception(ERR_ARG_NULL, "Argument 'syscall' is null.");
  }
  _fd = _sys->sysSocket(Domain_INET, Type_DGRAM, UDP);
  if (_fd == -1) {
    throw NetworkException(ERR, "Could not open socket.");
  }
  int trueflag = 1;
  int failed = _sys->sysSetSockOpt(_fd, Level_SOCKET, Option_BROADCAST,
                                   (void *)&trueflag, sizeof(trueflag));
  if (failed) {
    _sys->sysClose(_fd);
    throw NetworkException(ERR, "Could not configure socket for broadcast.");
  }
}

UDPSocket::~UDPSocket() { _sys->sysClose(_fd); }

unsigned short UDPSocket::port() { return _port; }

bool UDPSocket::send(const char *buffer, int len) {
  if (!buffer) {
    throw Exception(ERR_ARG_NULL, "Argument 'buffer' is null.");
  }

  SockAddrInet addr(_port, "255.255.255.255");
  int chunkStart = 0;
  int chunkSize = len > SOCKET_MAX_SEND_SIZE ? SOCKET_MAX_SEND_SIZE : len;
  int remainingLen = len;
  while (remainingLen > 0) {
    int sizeSent = _sys->sysSendTo(_fd, &buffer[chunkStart], chunkSize, 0,
                                   &addr, addr.len());
    if (sizeSent == -1) {
      throw NetworkException(ERR, "Send message failed.");
    }
    chunkStart += sizeSent;
    remainingLen -= sizeSent;
    chunkSize = remainingLen > SOCKET_MAX_SEND_SIZE ? SOCKET_MAX_SEND_SIZE
                                                    : remainingLen;
  }
  return true;
}

int UDPSocket::recv(char *buffer, int len, int timeout) {
  if (!buffer) {
    throw Exception(ERR_ARG_NULL, "Argument 'buffer' is null.");
  }

  bindSocket();

  if (timeout > 0) {
    PollFD pollFd(_fd, PollEvent::Event_IN);
    int pollReturn = _sys->sysPoll(&pollFd, 1, timeout);
    if (pollReturn == 0) {
      return 0;
    } else if (pollReturn > 0) {
      if (pollFd.revents & PollEvent::Event_IN) {
        return handleRecv(buffer, len);
      } else {
        return 0;
      }
    } else {
      throw NetworkException(ERR, "Poll syscall failed.");
    }
  } else {
    return handleRecv(buffer, len);
  }
}

int UDPSocket::read(char *buffer, int len, const bool *const cancel) {
  if (!buffer) {
    throw Exception(ERR_ARG_NULL, "Argument 'buffer' is null.");
  }

  int bufferIndex = 0;
  while (bufferIndex < len && (!cancel || !*cancel)) {
    bufferIndex +=
        recv(&buffer[bufferIndex], len - bufferIndex, SOCKET_READ_TIMOUT);
  }
  return bufferIndex;
}

void UDPSocket::bindSocket() {
  if (_isBound) {
    return;
  }

  SockAddrInet addr(_port);
  int failed = _sys->sysBind(_fd, &addr, addr.len());
  if (failed) {
    throw NetworkException(ERR, "Could not bind to network socket.");
  }
  _isBound = true;
}

int UDPSocket::handleRecv(char *buffer, int len) {
  int receivedBytes = _sys->sysRecvFrom(_fd, buffer, len);
  if (receivedBytes < 0) {
    throw NetworkException(ERR, "Receiving data failed.");
  }
  return receivedBytes;
}

}; // namespace level