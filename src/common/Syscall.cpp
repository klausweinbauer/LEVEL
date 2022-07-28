#include <Syscall.hpp>

#ifdef _WIN32
#include <WinSock2.h>
#elif __linux__
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace level {

Syscall::~Syscall() {}

int Syscall::sysPoll(PollFD *fds, nfds_l nfds, int timeout) {
#ifdef _WIN32
  return WSAPoll((pollfd *)fds, nfds, timeout);
#elif __linux__
  return poll(fds, nfds, timeout);
#endif
}

int Syscall::sysSocket(SockDomain domain, SockType type, Protocol protocol) {
#ifdef _WIN32
  SysWSASession::instance();
  return socket(domain, type, protocol);
#elif __linux__
  return socket(domain, type, protocol);
#endif
}

ssize_t Syscall::sysSendTo(int sockfd, const void *buf, size_t len, int flags,
                           const SockAddr *dest_addr, SockLen addrlen) {
#ifdef _WIN32
  return sendto(sockfd, (char *)buf, len, flags, (sockaddr *)dest_addr,
                addrlen);
#elif __linux__
  return sendto(sockfd, buf, len, flags, dest_addr, addrlen);
#endif
}

int Syscall::sysSetSockOpt(int socket, ProtocolLevel level,
                           SocketOption option_name, const void *option_value,
                           SockLen option_len) {
#ifdef _WIN32
  return setsockopt(socket, level, option_name, (char *)option_value,
                    option_len);
#elif __linux__
  return setsockopt(socket, level, option_name, option_value, option_len);
#endif
}

int Syscall::sysClose(int fd) {
#ifdef _WIN32
  return closesocket(fd);
#elif __linux__
  return close(fd);
#endif
}

int Syscall::sysBind(int sockfd, const SockAddr *addr, SockLen addrlen) {
#ifdef _WIN32
  return bind(sockfd, (sockaddr *)addr, addrlen);
#elif __linux__
  return bind(sockfd, addr, addrlen);
#endif
}

ssize_t Syscall::sysRecvFrom(int sockfd, void *buf, size_t len, int flags,
                             SockAddr *src_addr, SockLen *addrlen) {
#ifdef _WIN32
  return recvfrom(sockfd, (char *)buf, len, flags, (sockaddr *)src_addr,
                  (int *)addrlen);
#elif __linux__
  return recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
#endif
}

} // namespace level