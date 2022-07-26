#include <Syscall.hpp>

#ifdef _WIN32
#elif __linux__
#include <sys/socket.h>
#endif

namespace level {

int Syscall::posixPoll(struct pollfd *fds, nfds_t nfds, int timeout) {
#ifdef _WIN32

#elif __linux__
  return poll(fds, nfds, timeout);
#endif
}

int Syscall::posixSocket(int domain, int type, int protocol) {
#ifdef _WIN32
#elif __linux__
  return socket(domain, type, protocol);
#endif
}

ssize_t Syscall::posixSendTo(int sockfd, const void *buf, size_t len, int flags,
                             const struct sockaddr *dest_addr,
                             socklen_t addrlen) {
#ifdef _WIN32
#elif __linux__
  return sendto(sockfd, buf, len, flags, dest_addr, addrlen);
#endif
}

} // namespace level