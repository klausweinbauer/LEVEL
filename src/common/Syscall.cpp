#include <Syscall.hpp>

#ifdef _WIN32
#include <WinSock2.h>
#elif __linux__
#include <sys/socket.h>
#endif

namespace level {

int Syscall::posixPoll(pollfd_l *fds, nfds_l nfds, int timeout) {
#ifdef _WIN32
  return WSAPoll((pollfd*)fds, nfds, timeout);
#elif __linux__
  return poll(fds, nfds, timeout);
#endif
}

int Syscall::posixSocket(int domain, int type, int protocol) {
#if defined(_WIN32) || defined(__linux__)
  return socket(domain, type, protocol);
#endif
}

ssize_t Syscall::posixSendTo(int sockfd, const void *buf, size_t len, int flags, const sockaddr_l *dest_addr, socklen_l addrlen) {
#ifdef _WIN32
  return sendto(sockfd, (char*)buf, len, flags, (sockaddr*)dest_addr, addrlen);
#elif __linux__
  return sendto(sockfd, buf, len, flags, dest_addr, addrlen);
#endif
}

int Syscall::posixSetSockOpt(int socket, int level, int option_name,
                 const void *option_value, socklen_l option_len)
{
#ifdef _WIN32
  return setsockopt(socket, level, option_name, (char*)option_value, option_len);
#elif __linux__
#endif
}

int Syscall::posixClose(int fd) {
#ifdef _WIN32
  return closesocket(fd);
#elif __linux__
#endif
}

int Syscall::posixBind(int sockfd, const sockaddr_l *addr, socklen_l addrlen) {
#ifdef _WIN32
  return bind(sockfd, (sockaddr*)addr, addrlen);
#elif __linux__
#endif
}

ssize_t Syscall::posixRecvFrom(int sockfd, void *buf, size_t len, int flags, sockaddr_l *src_addr, socklen_l addrlen) {
#ifdef _WIN32
  return recvfrom(sockfd, (char*)buf, len, flags, (sockaddr*)src_addr, (int*)addrlen);
#elif __linux__
#endif
}

} // namespace level