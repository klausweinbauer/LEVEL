// Copyright 2022 Klaus Weinbauer
//
// This file is part of LEVEL.
//
// LEVEL is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// LEVEL is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LEVEL. If not, see <https://www.gnu.org/licenses/>.

#include <Syscall.hpp>
#include <chrono>
#include <cstring>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <thread>

using namespace level;

std::shared_ptr<ISyscall> Syscall_get() { return std::make_shared<Syscall>(); }

TEST(Syscall, OpenAndCloseUDPSocket) {
  auto sys = Syscall_get();
  int fd = sys->sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  ASSERT_EQ(0, sys->sysClose(fd));
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
  auto sys = Syscall_get();
  int fd = sys->sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  SockAddrInet addr(10000 + rand() % 50000);
  ASSERT_EQ(0, sys->sysBind(fd, &addr, addr.len()));
  ASSERT_EQ(0, sys->sysClose(fd));
}

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds milliseconds;

TEST(Syscall, PollTimeout) {
  auto sys = Syscall_get();
  int timeout = 100;
  int fd = sys->sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  PollFD pfd(fd, PollEvent::Event_IN);
  Clock::time_point t0 = Clock::now();
  ASSERT_EQ(0, sys->sysPoll(&pfd, 1, timeout));
  Clock::time_point t1 = Clock::now();
  milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
#ifdef _WIN32
  ASSERT_TRUE((timeout * 0.95) <= ms.count());
#elif __linux__
  ASSERT_TRUE(timeout <= ms.count());
#endif
  ASSERT_EQ(0, sys->sysClose(fd));
}

TEST(Syscall, SendTo) {
  auto sys = Syscall_get();
  std::string msg("Hello World!");
  int fd = sys->sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  SockAddrInet addr(10000 + rand() % 50000, "127.0.0.1");
  int ret =
      sys->sysSendTo(fd, msg.c_str(), msg.length() + 1, 0, &addr, addr.len());
  ASSERT_EQ(msg.length() + 1, ret);
  ASSERT_EQ(0, sys->sysClose(fd));
}

void Syscall_send(uint16_t port, int timeout, std::string msg) {
  auto sys = Syscall_get();
  SockAddrInet addr(port, "127.0.0.1");
  int fd = sys->sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  std::this_thread::sleep_for(std::chrono::milliseconds(timeout / 2));
  int ret =
      sys->sysSendTo(fd, msg.c_str(), msg.length() + 1, 0, &addr, addr.len());
  ASSERT_EQ(msg.length() + 1, ret);
  ASSERT_EQ(0, sys->sysClose(fd));
}

TEST(Syscall, PollAndRecvFrom) {
  uint16_t port = 10000 + rand() % 50000;
  std::string msg("Hello World!");
  auto sys = Syscall_get();
  int timeout = 200;
  int fd = sys->sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  SockAddrInet addr(port);
  ASSERT_EQ(0, sys->sysBind(fd, &addr, addr.len()));
  PollFD pfd(fd, PollEvent::Event_IN);
  std::thread thread(Syscall_send, port, timeout, msg);
  ASSERT_EQ(1, sys->sysPoll(&pfd, 1, timeout));
  const int bufferSize = 256;
  char *buffer[bufferSize];
  SockAddrInet srcAddr;
  SockLen srcAddrLen = srcAddr.len();
  int ret = sys->sysRecvFrom(fd, (void *)buffer, bufferSize, 0, &srcAddr,
                             &srcAddrLen);
  ASSERT_EQ(msg.length() + 1, ret);
  ASSERT_EQ(0, msg.compare(std::string((char *)buffer)));
  ASSERT_EQ("127.0.0.1", srcAddr.addr());
  ASSERT_EQ(0, sys->sysClose(fd));
  thread.join();
}

TEST(Syscall, CheckDefaultInitializationOfInetSocketAddress) {
  SockAddrInet socketAddr;
  ASSERT_EQ("0.0.0.0", socketAddr.addr());
  ASSERT_EQ(0, socketAddr.port());
}

TEST(Syscall, GetAddressFromInetSocketAddress) {
  std::string strAddr("1.2.3.4");
  uint16_t port = 4321;
  SockAddrInet socketAddr(port, strAddr);
  ASSERT_EQ(0, socketAddr.addr().compare(strAddr));
  ASSERT_EQ(port, socketAddr.port());
}

TEST(Syscall, SetSocketOptionBroadcast) {
  auto sys = Syscall_get();
  int fd = sys->sysSocket(Domain_INET, Type_DGRAM, UDP);
  ASSERT_TRUE(fd > 2);
  int trueflag = 1;
  int ret = sys->sysSetSockOpt(fd, Level_SOCKET, Option_BROADCAST,
                               (void *)&trueflag, sizeof(trueflag));
  ASSERT_EQ(0, ret);
  ASSERT_EQ(0, sys->sysClose(fd));
}

TEST(Syscall, UDPPacketSizeBound) {
  uint16_t port = 10000 + rand() % 50000;
  const int bufferSize = 1 << 16;
  char *buffer = new char[bufferSize]();
  auto sys = Syscall_get();
  int fd = sys->sysSocket(Domain_INET, Type_DGRAM, UDP);
  SockAddrInet addr(port, "127.0.0.1");
  int sendResult = sys->sysSendTo(fd, buffer, bufferSize, 0, &addr, addr.len());
  ASSERT_TRUE(sendResult < bufferSize);
  ASSERT_EQ(0, sys->sysClose(fd));
  delete[] buffer;
}

TEST(Syscall, SocketAddressInitialization) {
  uint16_t port = rand() % 65535;
  SockAddrInet addr1(port);
  sockaddr_in addr2{};
  addr2.sin_family = AF_INET;
  addr2.sin_port = htons(port);
#ifdef WIN32
  addr2.sin_addr.s_addr = INADDR_ANY;
#elif __linux__
  addr2.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
  ASSERT_EQ(sizeof(SockAddrInet), sizeof(sockaddr_in));
  char *addr1Raw = (char *)&addr1;
  char *addr2Raw = (char *)&addr2;
  for (int i = 0; i < (int)sizeof(SockAddrInet); i++) {
    ASSERT_EQ(addr1Raw[i], addr2Raw[i]);
  }
}

TEST(Syscall, SocketAddressInitializationWithIP) {
  uint16_t port = rand() % 65535;
  std::string ip("1.2.33.44");
  SockAddrInet addr1(port, ip);
  sockaddr_in addr2{};
  addr2.sin_family = AF_INET;
  addr2.sin_port = htons(port);
#ifdef WIN32
  addr2.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
#elif __linux__
  addr2.sin_addr.s_addr = inet_addr(ip.c_str());
#endif
  ASSERT_EQ(sizeof(SockAddrInet), sizeof(sockaddr_in));
  char *addr1Raw = (char *)&addr1;
  char *addr2Raw = (char *)&addr2;
  for (int i = 0; i < (int)sizeof(SockAddrInet); i++) {
    ASSERT_EQ(addr1Raw[i], addr2Raw[i]);
  }
}