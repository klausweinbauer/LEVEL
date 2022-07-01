#include <NetworkException.hpp>
#include <UDPSocket.hpp>
#include <sstream>
#include <string.h>

#ifdef _WIN32
#include <WS2tcpip.h>
#else
#include <arpa/inet.h>
#include <stdexcept>
#include <unistd.h>

#endif

namespace c2x {
#ifdef _WIN32
WSASession::WSASession() {
  int ret = WSAStartup(MAKEWORD(2, 2), &data_);
  if (ret != 0)
    throw std::system_error(WSAGetLastError(), std::system_category(),
                            "WSAStartup Failed");
}

WSASession::~WSASession() { WSACleanup(); }
#endif

#ifdef _WIN32
UDPSocket::UDPSocket() : _recvFail(false) {
  _sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (_sock == INVALID_SOCKET)
    throw std::system_error(WSAGetLastError(), std::system_category(),
                            "Error opening socket");
  int trueflag = 1;
  int res = setsockopt(_sock, SOL_SOCKET, SO_BROADCAST, (char *)&trueflag,
                       sizeof trueflag);
  if (res < 0)
    throw std::system_error(WSAGetLastError(), std::system_category(),
                            "Error set broadcast options");
}
#else
UDPSocket::UDPSocket() : _recvFail(false) {
  _sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (_sock < 0) {
    std::stringstream errMsg;
    errMsg << "Opening UDP socket failed. Error message: " << strerror(errno)
           << std::endl;
    throw NetworkException(ERR, errMsg.str().c_str());
  }

  int trueflag = 1;
  if (setsockopt(_sock, SOL_SOCKET, SO_BROADCAST, &trueflag, sizeof(trueflag)) <
      0) {
    ::close(_sock);
    std::stringstream errMsg;
    errMsg << "Setting UDP socket options failed. Error message: "
           << strerror(errno) << std::endl;
    throw NetworkException(ERR, errMsg.str().c_str());
  }
}
#endif

#ifdef _WIN32
UDPSocket::~UDPSocket() { closesocket(_sock); }
#else
UDPSocket::~UDPSocket() { ::close(_sock); }
#endif

void UDPSocket::sendTo(unsigned short port, const char *buffer, int len,
                       int flags) {
  sockaddr_in add{};
  add.sin_family = AF_INET;
  add.sin_addr.s_addr = inet_addr("255.255.255.255");
  add.sin_port = htons(port);

#ifdef _WIN32
  int ret = sendto(_sock, buffer, len, flags,
                   reinterpret_cast<SOCKADDR *>(&add), sizeof(add));
  if (ret < 0) {
    throw std::system_error(WSAGetLastError(), std::system_category(),
                            "sendto failed");
  }
#else
  int ret = sendto(_sock, buffer, len, flags,
                   reinterpret_cast<sockaddr *>(&add), sizeof(add));
  if (ret < 0) {
    std::stringstream errMsg;
    errMsg << "Sending UDP packet failed. Error message: " << strerror(errno)
           << std::endl;
    throw NetworkException(ERR, errMsg.str().c_str());
  }
#endif
}

int UDPSocket::recvFrom(char *buffer, int len, sockaddr_in *from_addr,
                        int flags) {
  _recvFail = false;
  int ret = recvfrom(_sock, buffer, len, flags, nullptr, nullptr);
  if (ret < 0) {
    if (!_recvFail) {
#ifdef _WIN32
      throw std::system_error(WSAGetLastError(), std::system_category(),
                              "recvfrom failed");
#else
      std::stringstream errMsg;
      errMsg << "Receiving UDP packet failed. Error message: "
             << strerror(errno) << std::endl;
      throw NetworkException(ERR, errMsg.str().c_str());
#endif
    }
  } else {
    // make the buffer zero terminated
    buffer[ret] = 0;
  }

  return ret;
}

void UDPSocket::close() {
#ifdef WIN32
  _recvFail = true;
  shutdown(_sock, SD_BOTH);
#else
  shutdown(_sock, SHUT_RDWR);
#endif
}

void UDPSocket::bindSocket(unsigned short port) {
  sockaddr_in add{};
  add.sin_family = AF_INET;
  add.sin_addr.s_addr = htonl(INADDR_ANY);
  add.sin_port = htons(port);

#ifdef _WIN32
  int ret = bind(_sock, reinterpret_cast<SOCKADDR *>(&add), sizeof(add));
  if (ret < 0) {
    throw std::system_error(WSAGetLastError(), std::system_category(),
                            "Bind failed");
  }
#else
  int ret = bind(_sock, reinterpret_cast<sockaddr *>(&add), sizeof(add));
  if (ret < 0) {
    std::stringstream errMsg;
    errMsg << "Binding to UDP socket failed. Error message: " << strerror(errno)
           << std::endl;
    throw NetworkException(ERR, errMsg.str().c_str());
  }
#endif
}
}; // namespace c2x