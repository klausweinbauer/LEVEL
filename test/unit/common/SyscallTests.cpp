#include <Syscall.hpp>
#include <chrono>
#include <cstring>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <thread>

using namespace level;

TEST(Syscall, OpenAndCloseUDPSocket) {
  Syscall sys;
  int fd = sys.sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  ASSERT_EQ(0, sys.sysClose(fd));
}

TEST(Syscall, SocketAddressSize) {
  ASSERT_EQ(sizeof(SockAddr), sizeof(sockaddr));
  ASSERT_EQ(sizeof(SockAddrInet), sizeof(sockaddr_in));
  ASSERT_EQ(sizeof(SockAddr), sizeof(SockAddrInet));
  SockAddr addr;
  ASSERT_EQ(sizeof(sockaddr), addr.len());
  SockAddrInet addrInet(1234);
  ASSERT_EQ(sizeof(sockaddr_in), addrInet.len());
}

TEST(Syscall, BindSocket) {
  Syscall sys;
  int fd = sys.sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  SockAddrInet addr(10000 + rand() % 50000);
  ASSERT_EQ(0, sys.sysBind(fd, &addr, addr.len()));
  ASSERT_EQ(0, sys.sysClose(fd));
}

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds milliseconds;

TEST(Syscall, PollTimeout) {
  Syscall sys;
  int timeout = 100;
  int fd = sys.sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  PollFD pfd(fd, PollEvent::IN);
  Clock::time_point t0 = Clock::now();
  ASSERT_EQ(0, sys.sysPoll(&pfd, 1, timeout));
  Clock::time_point t1 = Clock::now();
  milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
  ASSERT_TRUE(timeout <= ms.count());
  ASSERT_EQ(0, sys.sysClose(fd));
}

TEST(Syscall, SendTo) {
  Syscall sys;
  std::string msg("Hello World!");
  int fd = sys.sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  SockAddrInet addr("127.0.0.1", 10000 + rand() % 50000);
  int ret =
      sys.sysSendTo(fd, msg.c_str(), msg.length() + 1, 0, &addr, addr.len());
  ASSERT_EQ(msg.length() + 1, ret);
  ASSERT_EQ(0, sys.sysClose(fd));
}

void Syscall_send(uint16_t port, int timeout, std::string msg) {
  Syscall sys;
  SockAddrInet addr("127.0.0.1", port);
  int fd = sys.sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  std::this_thread::sleep_for(std::chrono::milliseconds(timeout / 2));
  int ret =
      sys.sysSendTo(fd, msg.c_str(), msg.length() + 1, 0, &addr, addr.len());
  ASSERT_EQ(msg.length() + 1, ret);
  ASSERT_EQ(0, sys.sysClose(fd));
}

TEST(Syscall, PollAndRecvFrom) {
  uint16_t port = 10000 + rand() % 50000;
  std::string msg("Hello World!");
  Syscall sys;
  int timeout = 200;
  int fd = sys.sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  SockAddrInet addr(port);
  ASSERT_EQ(0, sys.sysBind(fd, &addr, addr.len()));
  PollFD pfd(fd, PollEvent::IN);
  std::thread thread(Syscall_send, port, timeout, msg);
  ASSERT_EQ(1, sys.sysPoll(&pfd, 1, timeout));
  const int bufferSize = 256;
  char *buffer[bufferSize];
  SockAddrInet srcAddr;
  SockLen srcAddrLen = srcAddr.len();
  int ret =
      sys.sysRecvFrom(fd, (void *)buffer, bufferSize, 0, &srcAddr, &srcAddrLen);
  ASSERT_EQ(msg.length() + 1, ret);
  ASSERT_EQ(0, msg.compare(std::string((char *)buffer)));
  ASSERT_EQ("127.0.0.1", srcAddr.addr());
  ASSERT_EQ(0, sys.sysClose(fd));
  thread.join();
}

TEST(Syscall, GetAddressFromUninitializedInetSocketAddress) {
  SockAddrInet socketAddr;
  std::string addr = socketAddr.addr();
  ASSERT_EQ(0, addr.length());
}

TEST(Syscall, GetAddressFromInetSocketAddress) {
  std::string strAddr("1.2.3.4");
  uint16_t port = 4321;
  SockAddrInet socketAddr(strAddr, port);
  ASSERT_EQ(0, socketAddr.addr().compare(strAddr));
  ASSERT_EQ(port, socketAddr.port());
}

TEST(Syscall, SetSocketOptionBroadcast) {
  Syscall sys;
  int fd = sys.sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  int trueflag = 1;
  int ret = sys.sysSetSockOpt(fd, Level_SOCKET, Option_BROADCAST,
                              (void *)&trueflag, sizeof(trueflag));
  ASSERT_EQ(0, ret);
  ASSERT_EQ(0, sys.sysClose(fd));
}