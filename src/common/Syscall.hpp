#pragma once

#ifdef _WIN32

#elif __linux__
#include <poll.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/types.h>
#endif

namespace level {

#ifdef _WIN32
typedef unsigned long int nfds_t;
typedef unsigned int socklen_t;

struct pollfd {
  int fd;
  short events;
  short revents;
};
#elif __linux__
#endif

class Syscall {

  /**
   * @brief Wait for some event on a file descriptor.
   *
   * @param fds Array of file descriptors to be monitored.
   * @param nfds Number of elements in the fds array.
   * @param timeout Argument specifies the number of milliseconds that poll()
   * should block waiting for a file descriptor to become ready.
   * @return int A positive value is the number of elements in fds with nonzero
   * revents fields. Zero indicates the system call timed out, and -1 is
   * returned on error.
   */
  int posixPoll(struct pollfd *fds, nfds_t nfds, int timeout);

  /**
   * @brief Creates an endpoint for communication and returns a file descriptor
   * that refers to that endpoint.
   *
   * @param domain The domain argument specifies a communication domain; this
   * selects the protocol family which will be used for communication.
   * @param type The socket has the indicated type, which specifies the
   * communication semantics.
   * @param protocol
   * @return int On success, a file descriptor for the new socket is returned.
   * On error, -1 is returned.
   */
  int posixSocket(int domain, int type, int protocol);

  /**
   * @brief Is used to transmit a message to another socket.
   *
   * @param sockfd Is the file descriptor of the sending socket.
   * @param buf The message to transmit.
   * @param len Length of the message.
   * @param flags This argument is the bitwise OR of zero or more flags.
   * @param dest_addr The address of the target.
   * @param addrlen The size of the dest_addr parameter.
   * @return ssize_t On success, these calls return the number of bytes sent. On
   * error, -1 is returned.
   */
  ssize_t posixSendTo(int sockfd, const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen);
};

} // namespace level