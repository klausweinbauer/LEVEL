#include <Mocks.hpp>
#include <NetworkException.hpp>
#include <UDPSocket.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::Throw;

namespace level {
namespace UDPSocketTests {

int sendToDataSum = 0;

ssize_t mSendTo(int sockfd, const void *buf, size_t len, int flags,
                const SockAddr *dest_addr, SockLen addrle) {
  const int failingSize = 61000;
  if (len > failingSize || sockfd < 2 || dest_addr == nullptr) {
    return -1;
  }
  sendToDataSum += len;
  return len;
}

std::shared_ptr<MSyscall> getSys(int fd = 2) {
  auto sys = std::make_shared<MSyscall>();
  ON_CALL(*sys, sysSocket(_, _, _)).WillByDefault(Return(fd));
  ON_CALL(*sys, sysSendTo(_, _, _, _, _, _)).WillByDefault(Invoke(mSendTo));
  return sys;
}

unsigned short randPort() { return (unsigned short)rand() % UINT16_MAX; }

std::shared_ptr<UDPSocket>
getSocket(unsigned short port = 0, std::shared_ptr<ISyscall> syscall = nullptr,
          int fd = 2) {
  if (port == 0) {
    port = randPort();
  }
  if (!syscall) {
    syscall = getSys(fd);
  }
  auto socket = std::make_shared<UDPSocket>(port, syscall);
  return socket;
}

} // namespace UDPSocketTests
} // namespace level

using namespace level;
using namespace level::UDPSocketTests;

TEST(UDPSocket, SuccessfulInitialization) {
  auto port = randPort();
  auto fd = rand() % 1024;
  auto sys = getSys();
  EXPECT_CALL(*sys, sysSocket(Domain_INET, Type_DGRAM, UDP))
      .WillOnce(Return(fd));
  EXPECT_CALL(*sys,
              sysSetSockOpt(fd, Level_SOCKET, Option_BROADCAST, nullptr, 0))
      .WillOnce(Return(0));
  auto socket = getSocket(port, sys);
  ASSERT_EQ(port, socket->port());
}

TEST(UDPSocket, ThrowIfSyscallIsNull) {
  auto port = randPort();
  ASSERT_THROW(UDPSocket(port, std::shared_ptr<ISyscall>(nullptr)), Exception);
}

TEST(UDPSocket, OpeningSocketFailed) {
  auto port = randPort();
  auto sys = getSys();
  EXPECT_CALL(*sys, sysSocket(Domain_INET, Type_DGRAM, UDP))
      .WillOnce(Return(-1));
  ASSERT_THROW(getSocket(port, sys), NetworkException);
}

TEST(UDPSocket, SetSocketOptionFailed) {
  auto port = randPort();
  auto sys = getSys();
  auto fd = rand() % 1024;
  EXPECT_CALL(*sys, sysSocket(Domain_INET, Type_DGRAM, UDP))
      .WillOnce(Return(fd));
  EXPECT_CALL(*sys, sysSetSockOpt(fd, _, _, _, _)).WillOnce(Return(-1));
  EXPECT_CALL(*sys, sysClose(fd)).Times(1);
  ASSERT_THROW(getSocket(port, sys), NetworkException);
}

TEST(UDPSocket, ClosesSocketInDestruction) {
  auto port = randPort();
  auto sys = getSys();
  auto fd = rand() % 1024;
  EXPECT_CALL(*sys, sysSocket(Domain_INET, Type_DGRAM, UDP))
      .WillOnce(Return(fd));
  EXPECT_CALL(*sys, sysClose(fd)).Times(1);
  { ASSERT_NO_THROW(getSocket(port, sys)); }
}

TEST(UDPSocket, SendMessage) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  const int bufferSize = 64;
  char buffer[64];
  EXPECT_CALL(*sys, sysSendTo(_, buffer, bufferSize, _, _, _))
      .Times(AtLeast(1))
      .WillRepeatedly(ReturnArg<2>());
  ASSERT_TRUE(socket->send(buffer, bufferSize));
}

TEST(UDPSocket, SendLargeMessage) {
  const int bufferSize = 100000;
  char *buffer = new char[bufferSize];
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  sendToDataSum = 0;
  ASSERT_TRUE(socket->send(buffer, bufferSize));
  ASSERT_EQ(bufferSize, sendToDataSum);
  delete[] buffer;
}

TEST(UDPSocket, SendMessageFails) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  const int bufferSize = 64;
  char *buffer = new char[bufferSize];
  EXPECT_CALL(*sys, sysSendTo(_, _, _, _, _, _)).WillRepeatedly(Return(-1));
  ASSERT_FALSE(socket->send(buffer, bufferSize));
}

TEST(UDPSocket, ThrowsIfBufferIsNull) {
  auto socket = getSocket();
  ASSERT_THROW(socket->send(nullptr, 0), Exception);
}
