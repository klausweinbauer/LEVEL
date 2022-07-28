#include <NetworkException.hpp>
#include <UDPSocket.hpp>
#include <cassert>
#include <memory>
#include <sstream>
#include <string.h>

// Though UDP can handle up to 65535kB (- header size) data packets, this is not
// practical. So we split data into chunks of SOCKET_MAX_SEND_SIZE.
#define SOCKET_MAX_SEND_SIZE 4096

namespace level {

UDPSocket::UDPSocket(unsigned short port, std::shared_ptr<ISyscall> syscall)
    : _sys(syscall), _fd(0), _addr(port) {
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

unsigned short UDPSocket::port() { return _addr.port(); }

bool UDPSocket::send(const char *buffer, int len) {
  if (!buffer) {
    throw Exception(ERR_ARG_NULL, "Argument 'buffer' is null.");
  }

  int chunkStart = 0;
  int chunkSize = len > SOCKET_MAX_SEND_SIZE ? SOCKET_MAX_SEND_SIZE : len;
  int remainingLen = len;
  while (remainingLen > 0) {
    int sizeSent = _sys->sysSendTo(_fd, &buffer[chunkStart], chunkSize, 0,
                                   &_addr, _addr.len());
    if (sizeSent == -1) {
      return false;
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
  if (timeout > 0) {
    PollFD pollFd(_fd, PollEvent::Event_IN);
    int pollReturn = _sys->sysPoll(&pollFd, 1, timeout);
    if (pollReturn == 0) {
      return 0;
    } else if (pollReturn > 0) {
      if (pollFd.revents == PollEvent::Event_IN) {
        int retResult = _sys->sysRecvFrom(_fd, buffer, len);
        return retResult;
      } else {
        return 0;
      }
    } else {
      throw Exception(ERR, "Poll syscall failed.");
    }
  } else {
    return _sys->sysRecvFrom(_fd, buffer, len);
  }
}

bool UDPSocket::read(char *buffer, int len, bool *cancel) {}

void UDPSocket::bindSocket() {}

}; // namespace level