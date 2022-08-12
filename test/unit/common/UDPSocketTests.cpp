#include <Mocks.hpp>
#include <NetworkException.hpp>
#include <UDPSocket.hpp>
#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;
using ::testing::Throw;
using ::testing::Values;

#define PI 3.1415926535

namespace level {
namespace UDPSocketTests {

int sendToDataSum = 0;

const int gBufferLen = 64;
char gBuffer[gBufferLen];

int mSendTo(int sockfd, const void *buf, size_t len, int flags,
            const SockAddr *dest_addr, SockLen addrle) {
  const int failingSize = 61000;
  if (len > failingSize || sockfd < 2 || dest_addr == nullptr) {
    return -1;
  }
  sendToDataSum += len;
  return len;
}

std::shared_ptr<NiceMock<MSyscall>> getSys(int fd = 2, int readSize = 10) {
  auto sys = std::make_shared<NiceMock<MSyscall>>();
  ON_CALL(*sys, sysSocket(_, _, _)).WillByDefault(Return(fd));
  ON_CALL(*sys, sysSendTo(_, _, _, _, _, _)).WillByDefault(Invoke(mSendTo));
  ON_CALL(*sys, sysPoll(_, _, _))
      .WillByDefault(DoAll(Invoke([](PollFD *fds, nfds_l nfds, int timeout) {
                             assert(fds != nullptr);
                             assert(nfds > 0);
                             fds->revents = fds->events;
                           }),
                           Return(1)));
  ON_CALL(*sys, sysRecvFrom(_, _, _, _, _, _))
      .WillByDefault(
          Invoke([readSize](int sockfd, void *buf, size_t len, int flags,
                            SockAddr *src_addr, SockLen *addrlen) {
            assert(sockfd > 0);
            assert(buf != nullptr);
            return (int)len < readSize ? (int)len : readSize;
          }));
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

class RecvTest : public testing::TestWithParam<PollEvent> {};
class ReadTest : public testing::TestWithParam<PollEvent> {};

std::function<std::string(PollEvent e)> pollEventToString = [](PollEvent e) {
  switch (e) {
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
};

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
  EXPECT_CALL(*sys, sysSendTo(_, gBuffer, gBufferLen, _, _, _))
      .Times(AtLeast(1))
      .WillRepeatedly(ReturnArg<2>());
  ASSERT_TRUE(socket->send(gBuffer, gBufferLen));
}

TEST(UDPSocket, SendLargeMessage) {
  double chunks = PI * PI;
  const int bufferLen = SOCKET_MAX_SEND_SIZE * chunks;
  char *buffer = new char[bufferLen];
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  sendToDataSum = 0;
  EXPECT_CALL(*sys, sysSendTo(_, _, _, _, _, _))
      .Times(AtLeast((int)floor(chunks)));
  ASSERT_TRUE(socket->send(buffer, bufferLen));
  ASSERT_EQ(bufferLen, sendToDataSum);
  delete[] buffer;
}

TEST(UDPSocket, SendMessageFails) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  EXPECT_CALL(*sys, sysSendTo(_, _, _, _, _, _)).WillRepeatedly(Return(-1));
  ASSERT_THROW(socket->send(gBuffer, gBufferLen), NetworkException);
}

TEST(UDPSocket, ThrowsIfSendBufferIsNull) {
  auto socket = getSocket();
  ASSERT_THROW(socket->send(nullptr, 0), Exception);
}

TEST(UDPSocket, ReceiveData) {
  auto port = randPort();
  int readSize = gBufferLen * 0.7;
  auto sys = getSys(2, readSize);
  auto socket = getSocket(port, sys);
  ASSERT_EQ(readSize, socket->recv(gBuffer, gBufferLen));
}

TEST(UDPSocket, ReceiveDataFails) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  EXPECT_CALL(*sys, sysRecvFrom(_, gBuffer, gBufferLen, _, _, _))
      .WillOnce(Return(-1));
  ASSERT_THROW(socket->recv(gBuffer, gBufferLen), NetworkException);
}

TEST(UDPSocket, ReceiveDataWithTimeoutFails) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  EXPECT_CALL(*sys, sysRecvFrom(_, gBuffer, gBufferLen, _, _, _))
      .WillOnce(Return(-1));
  ASSERT_THROW(socket->recv(gBuffer, gBufferLen, 100), NetworkException);
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
  EXPECT_CALL(*sys, sysPoll(_, 1, timeout)).WillOnce(Return(0));
  ASSERT_EQ(0, socket->recv(gBuffer, gBufferLen, timeout));
}

TEST(UDPSocket, UseTimeoutAndReceiveData) {
  auto port = randPort();
  int fd = 3;
  const int recvSize = 10;
  auto sys = getSys(fd, recvSize);
  auto socket = getSocket(port, sys);
  int timeout = 100;
  EXPECT_CALL(*sys, sysPoll(_, 1, timeout))
      .WillOnce(DoAll(Invoke([](PollFD *fds, nfds_l nfds, int timeout) {
                        fds->revents = PollEvent::Event_IN;
                      }),
                      Return(1)));
  ASSERT_EQ(recvSize, socket->recv(gBuffer, gBufferLen, timeout));
}

TEST_P(RecvTest, DontBlockRecv) {
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
    UDPSocket, RecvTest,
    Values(PollEvent::Event_ERR, PollEvent::Event_HUP, PollEvent::Event_NVAL,
           PollEvent::Event_OUT, PollEvent::Event_PRI),
    [](const ::testing::TestParamInfo<RecvTest::ParamType> &info) {
      return pollEventToString(info.param);
    });

TEST(UDPSocket, TimeoutFailedInReceive) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  int timeout = 100;
  EXPECT_CALL(*sys, sysPoll(_, 1, timeout)).WillOnce(Return(-1));
  EXPECT_CALL(*sys, sysRecvFrom(_, _, _, _, _, _)).Times(0);
  ASSERT_THROW(socket->recv(gBuffer, gBufferLen, timeout), NetworkException);
}

TEST(UDPSocket, BindSocketSuccessfulInReceive) {
  auto port = randPort();
  int fd = rand() % 1024;
  auto sys = getSys(fd);
  auto socket = getSocket(port, sys);
  const SockAddr *addr;
  EXPECT_CALL(*sys, sysBind(fd, _, _))
      .WillOnce(DoAll(SaveArg<1>(&addr), Return(0)));
  ASSERT_NO_THROW(socket->recv(gBuffer, gBufferLen));
  ASSERT_TRUE(addr != nullptr);
}

TEST(UDPSocket, CallRecvMultipleTimes) {
  auto port = randPort();
  int fd = rand() % 1024;
  auto sys = getSys(fd);
  auto socket = getSocket(port, sys);
  EXPECT_CALL(*sys, sysBind(fd, _, _))
      .WillOnce(Return(0))
      .WillRepeatedly(Return(-1));
  ASSERT_NO_THROW(socket->recv(gBuffer, gBufferLen));
  ASSERT_NO_THROW(socket->recv(gBuffer, gBufferLen));
}

TEST(UDPSocket, ThrowOnBindErrorInReceive) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  EXPECT_CALL(*sys, sysBind(_, _, _)).WillRepeatedly(Return(-1));
  ASSERT_THROW(socket->recv(gBuffer, gBufferLen), NetworkException);
}

TEST(UDPSocket, ReadData) {
  double readChunkFactor = PI / 9.0;
  const int readChunkSize = gBufferLen * readChunkFactor;
  auto port = randPort();
  auto sys = getSys(2, readChunkSize);
  auto socket = getSocket(port, sys);
  EXPECT_CALL(*sys, sysRecvFrom(_, _, _, _, _, _))
      .Times(AtLeast(ceil(1 / readChunkFactor)));
  ASSERT_EQ(gBufferLen, socket->read(gBuffer, gBufferLen));
}

TEST(UDPSocket, ReadDataFails) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  EXPECT_CALL(*sys, sysRecvFrom(_, gBuffer, gBufferLen, _, _, _))
      .WillOnce(Return(-1));
  ASSERT_THROW(socket->read(gBuffer, gBufferLen), NetworkException);
}

TEST(UDPSocket, ThrowsIfReadBufferIsNull) {
  auto socket = getSocket();
  ASSERT_THROW(socket->read(nullptr, 0), Exception);
}

TEST(UDPSocket, TriggerTimeoutInRead) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  EXPECT_CALL(*sys, sysPoll(_, _, _))
      .WillOnce(Return(0))
      .WillRepeatedly(DoAll(Invoke([](PollFD *fds, nfds_l nfds, int timeout) {
                              assert(fds != nullptr);
                              assert(nfds > 0);
                              fds->revents = fds->events;
                            }),
                            Return(1)));
  ASSERT_EQ(gBufferLen, socket->read(gBuffer, gBufferLen));
}

TEST(UDPSocket, TimeoutFailedInRead) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  EXPECT_CALL(*sys, sysPoll(_, _, _)).WillOnce(Return(-1));
  EXPECT_CALL(*sys, sysRecvFrom(_, _, _, _, _, _)).Times(0);
  ASSERT_THROW(socket->read(gBuffer, gBufferLen), NetworkException);
}

TEST(UDPSocket, BindSocketSuccessfulInRead) {
  auto port = randPort();
  int fd = rand() % 1024;
  auto sys = getSys(fd);
  auto socket = getSocket(port, sys);
  const SockAddr *addr;
  EXPECT_CALL(*sys, sysBind(fd, _, _))
      .WillOnce(DoAll(SaveArg<1>(&addr), Return(0)));
  ASSERT_NO_THROW(socket->read(gBuffer, gBufferLen));
  ASSERT_TRUE(addr != nullptr);
}

TEST(UDPSocket, CallReadMultipleTimes) {
  auto port = randPort();
  int fd = rand() % 1024;
  auto sys = getSys(fd);
  auto socket = getSocket(port, sys);
  EXPECT_CALL(*sys, sysBind(fd, _, _))
      .WillOnce(Return(0))
      .WillRepeatedly(Return(-1));
  ASSERT_NO_THROW(socket->read(gBuffer, gBufferLen));
  ASSERT_NO_THROW(socket->read(gBuffer, gBufferLen));
}

TEST(UDPSocket, ThrowOnBindErrorInRead) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  EXPECT_CALL(*sys, sysBind(_, _, _)).WillRepeatedly(Return(-1));
  ASSERT_THROW(socket->read(gBuffer, gBufferLen), NetworkException);
}

TEST(UDPSocket, CancelRead) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  int readChunkSize = 10;
  int cancelAfterReadCalls = 3;
  int readCallsCounter = 0;
  bool cancelFlag = false;
  int readDelay = 20;
  EXPECT_CALL(*sys, sysPoll(_, _, _))
      .WillRepeatedly(
          Invoke([readDelay](PollFD *fds, nfds_l nfds, int timeout) {
            std::this_thread::sleep_for(std::chrono::milliseconds(readDelay));
            fds->revents = PollEvent::Event_IN;
            return 1;
          }));
  EXPECT_CALL(*sys, sysRecvFrom(_, _, _, _, _, _))
      .WillRepeatedly(Invoke(
          [readChunkSize, cancelAfterReadCalls, &cancelFlag, &readCallsCounter](
              int sockfd, void *buf, size_t len, int flags = 0,
              SockAddr *src_addr = nullptr, SockLen *addrlen = nullptr) {
            int readSize = (int)len < readChunkSize ? len : readChunkSize;
            readCallsCounter++;

            if (readCallsCounter == cancelAfterReadCalls) {
              cancelFlag = true;
            }

            return readSize;
          }));
  ASSERT_EQ(cancelAfterReadCalls * readChunkSize,
            socket->read(gBuffer, gBufferLen, &cancelFlag));
}

TEST_P(ReadTest, DontBlockRead) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);
  int callCounter = 0;
  const int maxCalls = 3;
  bool cancelFlag = false;
  EXPECT_CALL(*sys, sysPoll(_, 1, _))
      .WillRepeatedly(DoAll(Invoke([maxCalls, &callCounter, &cancelFlag](
                                       PollFD *fds, nfds_l nfds, int timeout) {
                              fds->revents = GetParam();
                              callCounter++;
                              if (callCounter == maxCalls) {
                                cancelFlag = true;
                              }
                            }),
                            Return(1)));
  EXPECT_CALL(*sys, sysRecvFrom(_, _, _, _, _, _)).Times(0);
  ASSERT_EQ(0, socket->read(gBuffer, gBufferLen, &cancelFlag));
}

INSTANTIATE_TEST_SUITE_P(
    UDPSocket, ReadTest,
    Values(PollEvent::Event_ERR, PollEvent::Event_HUP, PollEvent::Event_NVAL,
           PollEvent::Event_OUT, PollEvent::Event_PRI),
    [](const ::testing::TestParamInfo<ReadTest::ParamType> &info) {
      return pollEventToString(info.param);
    });

TEST(UDPSocket, ReadDataWhenMultipleFlagsArePresent) {
  auto port = randPort();
  auto sys = getSys();
  auto socket = getSocket(port, sys);

  EXPECT_CALL(*sys, sysPoll)
      .WillOnce(Invoke([](PollFD *fds, nfds_l nfds, int timeout) {
        assert(fds != nullptr);
        assert(nfds > 0);
        fds->revents = PollEvent::Event_IN | PollEvent::Event_OUT;
        return 1;
      }));

  EXPECT_CALL(*sys, sysRecvFrom(_, _, _, _, _, _)).Times(1);

  const int bufferSize = 100;
  char buffer[bufferSize];
  socket->recv(buffer, bufferSize, 100);
}