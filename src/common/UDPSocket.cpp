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

namespace level {

#ifdef _WIN32
WSASession::WSASession() {
  int ret = WSAStartup(MAKEWORD(2, 2), &data_);
  if (ret != 0)
    throw std::system_error(WSAGetLastError(), std::system_category(),
                            "WSAStartup Failed");
}

WSASession::~WSASession() { WSACleanup(); }
#endif

UDPSocket::UDPSocket(unsigned short port)
    : _port(port), _enableRecvException(true) {
  _sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

#ifdef _WIN32
  if (_sock == INVALID_SOCKET) {
#else
  if (_sock < 0) {
#endif
    std::stringstream errMsg;
    errMsg << "Opening UDP socket failed. Error message: " << strerror(errno)
           << std::endl;
    throw NetworkException(ERR, errMsg.str().c_str());
  }

  int trueflag = 1;
  int res = setsockopt(_sock, SOL_SOCKET, SO_BROADCAST, (char *)&trueflag,
                       sizeof trueflag);
  if (res < 0) {
#ifdef _WIN32
    closesocket(_sock);
#else
    ::close(_sock);
#endif
    std::stringstream errMsg;
    errMsg << "Setting UDP socket options failed. Error message: "
           << strerror(errno) << std::endl;
    throw NetworkException(ERR, errMsg.str().c_str());
  }
}

UDPSocket::~UDPSocket() {
#ifdef _WIN32
  closesocket(_sock);
#else
  ::close(_sock);
#endif
}

void UDPSocket::sendTo(const char *buffer, int len, int flags) {
  sockaddr_in add{};
  add.sin_family = AF_INET;
  add.sin_addr.s_addr = inet_addr("255.255.255.255");
  add.sin_port = htons(_port);

  int ret = sendto(_sock, buffer, len, flags,
                   reinterpret_cast<sockaddr *>(&add), sizeof(add));

  if (ret < 0) {
    std::stringstream errMsg;
    errMsg << "Sending UDP packet failed. Error message: " << strerror(errno)
           << std::endl;
    throw NetworkException(ERR, errMsg.str().c_str());
  }
}

int UDPSocket::recvFrom(char *buffer, int len, int flags) {
  _enableRecvException = true;
  int ret = recvfrom(_sock, buffer, len, flags, nullptr, nullptr);
  if (ret < 0) {
    if (_enableRecvException) {
      std::stringstream errMsg;
      errMsg << "Receiving UDP packet failed. Error message: "
             << strerror(errno) << std::endl;
      throw NetworkException(ERR, errMsg.str().c_str());
    }
  } else {
    // make the buffer zero terminated
    buffer[ret] = 0;
  }

  return ret;
}

void UDPSocket::close() {
  _enableRecvException = false;
#ifdef _WIN32
  // shutdown(_sock, SD_BOTH) did not work
  closesocket(_sock);
#else
  shutdown(_sock, SHUT_RDWR);
#endif
}

void UDPSocket::bindSocket() {
  sockaddr_in add{};
  add.sin_family = AF_INET;
  add.sin_addr.s_addr = htonl(INADDR_ANY);
  add.sin_port = htons(_port);

#ifdef _WIN32
  int ret = bind(_sock, reinterpret_cast<SOCKADDR *>(&add), sizeof(add));
#else
  int ret = bind(_sock, reinterpret_cast<sockaddr *>(&add), sizeof(add));
#endif

  if (ret < 0) {
    std::stringstream errMsg;
    errMsg << "Binding to UDP socket failed. Error message: " << strerror(errno)
           << std::endl;
    throw NetworkException(ERR, errMsg.str().c_str());
  }
}
}; // namespace level