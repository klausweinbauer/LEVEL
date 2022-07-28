#include <Mocks.hpp>
#include <NetworkException.hpp>
#include <UDPSocket.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;
using ::testing::Throw;
using ::testing::Values;

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
getSocket(unsigned short port = 0,
          std::shared_ptr<ISyscall> syscall = nullptr) {
  if (port == 0) {
    port = randPort();
  }
  if (!syscall) {
    syscall = getSys();
  }
  auto socket = std::make_shared<UDPSocket>(port, syscall);
  return socket;
}

class RecvWithTimeoutTest : public testing::TestWithParam<PollEvent> {};

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
  const void *trueflag;
  int trueflagSize = 0;
  EXPECT_CALL(*sys, sysSetSockOpt(fd, Level_SOCKET, Option_BROADCAST, _, _))
      .WillOnce(
          DoAll(SaveArg<3>(&trueflag), SaveArg<4>(&trueflagSize), Return(0)));
  auto socket = getSocket(port, sys);
  ASSERT_EQ(port, socket->port());
  ASSERT_NE(nullptr, trueflag);
  ASSERT_NE(0, trueflagSize);
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

TEST(UDPSocket, ThrowsIfSendBufferIsNull) {
  auto socket = getSocket();
  ASSERT_THROW(socket->send(nullptr, 0), Exception);
}

TEST(UDPSocket, ReceiveData) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  const int bufferSize = 64;
  int readSize = 50;
  char buffer[bufferSize];
  EXPECT_CALL(*sys, sysRecvFrom(_, buffer, bufferSize, _, _, _))
      .WillOnce(Return(readSize));
  ASSERT_EQ(readSize, socket->recv(buffer, bufferSize));
}

TEST(UDPSocket, ThrowsIfReceiveBufferIsNull) {
  auto socket = getSocket();
  ASSERT_THROW(socket->recv(nullptr, 0), Exception);
}

TEST(UDPSocket, TriggerTimeoutInReceive) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  int timeout = 100;
  const int bufferSize = 16;
  char buffer[bufferSize];
  EXPECT_CALL(*sys, sysPoll(_, 1, timeout)).WillOnce(Return(0));
  ASSERT_EQ(0, socket->recv(buffer, bufferSize, timeout));
}

TEST(UDPSocket, UseTimeoutAndReceiveData) {
  auto port = randPort();
  int fd = 3;
  auto sys = getSys(fd);
  auto socket = getSocket(port, sys);
  int timeout = 100;
  const int bufferSize = 16;
  const int recvSize = 10;
  char buffer[bufferSize];
  EXPECT_CALL(*sys, sysPoll(_, 1, timeout))
      .WillOnce(DoAll(Invoke([](PollFD *fds, nfds_l nfds, int timeout) {
                        fds->revents = PollEvent::Event_IN;
                      }),
                      Return(1)));
  EXPECT_CALL(*sys, sysRecvFrom(fd, buffer, bufferSize, _, _, _))
      .WillOnce(Return(recvSize));
  ASSERT_EQ(recvSize, socket->recv(buffer, bufferSize, timeout));
}

TEST_P(RecvWithTimeoutTest, DontCallRecvFromWithEvent) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  int timeout = 100;
  const int bufferSize = 16;
  char buffer[bufferSize];
  EXPECT_CALL(*sys, sysPoll(_, 1, timeout))
      .WillOnce(DoAll(Invoke([](PollFD *fds, nfds_l nfds, int timeout) {
                        fds->revents = GetParam();
                      }),
                      Return(1)));
  EXPECT_CALL(*sys, sysRecvFrom(_, _, _, _, _, _)).Times(0);
  ASSERT_EQ(0, socket->recv(buffer, bufferSize, timeout));
}

INSTANTIATE_TEST_SUITE_P(
    UDPSocket, RecvWithTimeoutTest,
    Values(PollEvent::Event_ERR, PollEvent::Event_HUP, PollEvent::Event_NVAL,
           PollEvent::Event_OUT, PollEvent::Event_PRI),
    [](const ::testing::TestParamInfo<RecvWithTimeoutTest::ParamType> &info) {
      switch (info.param) {
      case PollEvent::Event_ERR:
        return "ERR";
      case PollEvent::Event_HUP:
        return "HUP";
      case PollEvent::Event_NVAL:
        return "NVAL";
      case PollEvent::Event_OUT:
        return "OUT";
      case PollEvent::Event_PRI:
        return "PRI";
      default:
        return "unknown";
      }
    });

TEST(UDPSocket, TimeoutFailed) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  int timeout = 100;
  const int bufferSize = 16;
  char buffer[bufferSize];
  EXPECT_CALL(*sys, sysPoll(_, 1, timeout)).WillOnce(Return(-1));
  EXPECT_CALL(*sys, sysRecvFrom(_, _, _, _, _, _)).Times(0);
  ASSERT_THROW(socket->recv(buffer, bufferSize, timeout), Exception);
}