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
typedef unsigned long nfds_l;
typedef unsigned int socklen_l;
typedef long int ssize_t;


struct pollfd_l {
  int fd;
  short events;
  short revents; 
};

struct sockaddr_l {
  unsigned short int sa_family;
  char sa_data[14];
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
  int posixPoll(pollfd_l *fds, nfds_l nfds, int timeout);

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
                      const sockaddr_l *dest_addr, socklen_l addrlen);

  /**
  * @brief Sets socket options.
  *
  * @param socket File descriptor to set the options.
  * @param level Specifies the protocol level at which the option resides.
  * @param option_name Specifies a single option to set.
  * @param option_value 
  * @param option_len
  * @return int 0 is returned on successful completion and -1 otherwise.
  */
  int posixSetSockOpt(int socket, int level, int option_name,
                 const void *option_value, socklen_l option_len);

  /**
   * @brief Closes a file descriptor.
   * 
   * @param fd File descriptor to close.
   * @return int Returns zero on success and -1 on error.
   */
  int posixClose(int fd);

  /**
   * @brief Assigns the address specified by addr to the socket referred to by the file descriptor.
   * 
   * @param sockfd Socket file descriptor.
   * @param addr Address to assign.
   * @param addrlen Size of the address.
   * @return int Returns zero on success and -1 on error.
   */
  int posixBind(int sockfd, const sockaddr_l *addr, socklen_l addrlen);

  /**
   * @brief Used to receive messages from a socket.
   * 
   * @param sockfd Socket file descriptor.
   * @param buf Buffer for the received message.
   * @param len Length of the buffer.
   * @param flags Special flags.
   * @param src_addr Source address from where the message was received.
   * @param addrlen This is a value-result argument. Before the call, it should be initialized to the size of the buffer associated with src_addr. Upon return, addrlen is updated to contain the actual size of the source address.
   * @return ssize_t Returns the number of bytes received, or -1 if an error occurred.
   */
  ssize_t posixRecvFrom(int sockfd, void *buf, size_t len, int flags, sockaddr_l *src_addr, socklen_l addrlen);
};

} // namespace level