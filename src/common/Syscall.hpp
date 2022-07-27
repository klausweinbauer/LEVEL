#pragma once

#include <string>

#ifdef _WIN32
#elif __linux__
#include <arpa/inet.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/types.h>
#endif

namespace level {

#ifdef _WIN32
typedef unsigned long nfds_l;
typedef unsigned int SockLen;
typedef long int ssize_t;

struct PollFD {
  int fd;
  short events;
  short revents;
};

struct SockAddr {
  unsigned short int sa_family;
  char sa_data[14];
};
#elif __linux__
typedef nfds_t nfds_l;
typedef socklen_t SockLen;

enum SockDomain {
  Domain_LOCAL = AF_LOCAL,
  Domain_INET = AF_INET,
  Domain_INET6 = AF_INET6,
  Domain_PACKET = AF_PACKET,
  Domain_BLUETOOTH = AF_BLUETOOTH
};

enum SockType {
  Type_STREAM = SOCK_STREAM,
  Type_DGRAM = SOCK_DGRAM,
  Type_RAW = SOCK_RAW,
  Type_RDM = SOCK_RDM,
  Type_SEQPACKET = SOCK_SEQPACKET,
  Type_PACKET = SOCK_PACKET
};

enum Protocol { UDP = IPPROTO_UDP, TCP = IPPROTO_TCP, RAW = IPPROTO_RAW };

enum PollEvent {
  IN = POLLIN,
  PRI = POLLPRI,
  OUT = POLLOUT,
  RDHUP = POLLRDHUP,
  ERR = POLLERR,
  HUP = POLLHUP,
  NVAL = POLLNVAL
};

enum ProtocolLevel { Level_SOCKET = SOL_SOCKET };
enum SocketOption { Option_BROADCAST = SO_BROADCAST };

struct PollFD : pollfd {
  PollFD() {}
  PollFD(int fd, PollEvent event) {
    pollfd::fd = fd;
    pollfd::events = (int)event;
    pollfd::revents = 0;
  }
};

struct SockAddr : sockaddr {
  SockLen len() { return sizeof(SockAddr); }
};
struct SockAddrInet : SockAddr {
  SockAddrInet() {}

  SockAddrInet(uint16_t port) {
    sockaddr_in *addr = reinterpret_cast<sockaddr_in *>(this);
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = htonl(INADDR_ANY);
    addr->sin_port = htons(port);
  }

  SockAddrInet(std::string address, uint16_t port) {
    sockaddr_in *addr = reinterpret_cast<sockaddr_in *>(this);
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(address.c_str());
    addr->sin_port = htons(port);
  }

  SockLen len() { return sizeof(SockAddrInet); }

  std::string addr() {
    sockaddr_in *addr = reinterpret_cast<sockaddr_in *>(this);
    const int bufferSize = 64;
    char buffer[bufferSize];
    const char *result = inet_ntop(addr->sin_family, (void *)&addr->sin_addr,
                                   buffer, bufferSize);
    if (result != nullptr) {
      return std::string((char *)buffer);
    }
    return std::string();
  }

  uint16_t port() {
    sockaddr_in *addr = reinterpret_cast<sockaddr_in *>(this);
    return ntohs(addr->sin_port);
  }
};
#endif

class Syscall {

public:
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
  int sysPoll(PollFD *fds, nfds_l nfds, int timeout);

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
  int sysSocket(SockDomain domain, SockType type, Protocol protocol);

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
  ssize_t sysSendTo(int sockfd, const void *buf, size_t len, int flags,
                    const SockAddr *dest_addr, SockLen addrlen);

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
  int sysSetSockOpt(int socket, ProtocolLevel level, SocketOption option_name,
                    const void *option_value = nullptr, SockLen option_len = 0);

  /**
   * @brief Closes a file descriptor.
   *
   * @param fd File descriptor to close.
   * @return int Returns zero on success and -1 on error.
   */
  int sysClose(int fd);

  /**
   * @brief Assigns the address specified by addr to the socket referred to by
   * the file descriptor.
   *
   * @param sockfd Socket file descriptor.
   * @param addr Address to assign.
   * @param addrlen Size of the address.
   * @return int Returns zero on success and -1 on error.
   */
  int sysBind(int sockfd, const SockAddr *addr, SockLen addrlen);

  /**
   * @brief Used to receive messages from a socket.
   *
   * @param sockfd Socket file descriptor.
   * @param buf Buffer for the received message.
   * @param len Length of the buffer.
   * @param flags Special flags.
   * @param src_addr Source address from where the message was received.
   * @param addrlen This is a value-result argument. Before the call, it should
   * be initialized to the size of the buffer associated with src_addr. Upon
   * return, addrlen is updated to contain the actual size of the source
   * address.
   * @return ssize_t Returns the number of bytes received, or -1 if an error
   * occurred.
   */
  ssize_t sysRecvFrom(int sockfd, void *buf, size_t len, int flags = 0,
                      SockAddr *src_addr = nullptr, SockLen *addrlen = nullptr);
};

} // namespace level