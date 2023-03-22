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

#include <EncodeException.hpp>
#include <Mocks.hpp>
#include <SocketNAL.hpp>
#include <UDPSocket.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;

namespace level::SocketNALTests {

std::shared_ptr<NiceMock<MSocket>> getSocket() {
  return std::make_shared<NiceMock<MSocket>>();
}

template <typename T> std::shared_ptr<NiceMock<MEncoder<T>>> getEncoder() {
  return std::make_shared<NiceMock<MEncoder<T>>>();
}

template <typename T>
std::shared_ptr<NiceMock<MRecvHandler<T>>> getRecvHandler() {
  return std::make_shared<NiceMock<MRecvHandler<T>>>();
}

std::shared_ptr<NiceMock<MErrorHandler>> getErrHandler() {
  return std::make_shared<NiceMock<MErrorHandler>>();
}

} // namespace level::SocketNALTests

using namespace level;
using namespace level::SocketNALTests;

TEST(SocketNAL, SuccessfulConstruction) {
  ASSERT_NO_THROW(SocketNAL<int> nal(getSocket(), getSocket(),
                                     getEncoder<int>(), getRecvHandler<int>(),
                                     getErrHandler()));
}

TEST(SocketNAL, ThrowWhenSendSocketIsNull) {
  ASSERT_THROW(SocketNAL<int> nal(nullptr, getSocket(), getEncoder<int>(),
                                  getRecvHandler<int>(), getErrHandler()),
               Exception);
}

TEST(SocketNAL, ThrowWhenRecvSocketIsNull) {
  ASSERT_THROW(SocketNAL<int> nal(getSocket(), nullptr, getEncoder<int>(),
                                  getRecvHandler<int>(), getErrHandler()),
               Exception);
}

TEST(SocketNAL, ThrowWhenEncoderIsNull) {
  ASSERT_THROW(SocketNAL<int> nal(getSocket(), getSocket(), nullptr,
                                  getRecvHandler<int>(), getErrHandler()),
               Exception);
}

TEST(SocketNAL, SendMessage) {
  int msg;
  std::vector<char> encodedMsg{(char)(rand() % 255), (char)(rand() % 255)};
  auto encoder = getEncoder<int>();
  EXPECT_CALL(*encoder, encode(&msg)).WillOnce(Return(encodedMsg));

  char buffer[2];
  auto sendSocket = getSocket();
  EXPECT_CALL(*sendSocket, send(_, 2))
      .WillOnce(Invoke([&buffer](const char *b, int l) {
        buffer[0] = b[0];
        buffer[1] = b[1];
        return true;
      }));

  SocketNAL<int> nal(sendSocket, getSocket(), encoder, getRecvHandler<int>(),
                     getErrHandler());
  ASSERT_NO_THROW(nal.send(&msg));
  ASSERT_EQ(encodedMsg[0], buffer[0]);
  ASSERT_EQ(encodedMsg[1], buffer[1]);
}

TEST(SocketNAL, ReceiveMessage) {
  std::vector<char> encodedMsg{(char)(rand() % 255), (char)(rand() % 255)};
  int msg;
  bool waitFlag = true;

  auto encoder = getEncoder<int>();
  EXPECT_CALL(*encoder, decode(_, 2))
      .WillOnce(Invoke([&encodedMsg, &msg](const char *buffer, int bufferSize) {
        int encodedMsgSize = (int)encodedMsg.size();
        if (bufferSize > 0 && bufferSize == encodedMsgSize) {
          for (int i = 0; i < bufferSize; i++) {
            if (buffer[i] != encodedMsg[i]) {
              throw Exception();
            }
          }
          return &msg;
        } else {
          throw EncodeException();
        }
      }));

  auto recvSocket = getSocket();
  EXPECT_CALL(*recvSocket, recv(_, _, _))
      .WillOnce(
          Invoke([&encodedMsg, &waitFlag](char *buffer, int len, int timeout) {
            int i = 0;
            int msgSize = (int)encodedMsg.size();
            while (i < len && i < msgSize) {
              buffer[i] = encodedMsg[i];
              i++;
            }
            waitFlag = false;
            return i;
          }))
      .WillRepeatedly(Return(0));

  int *receivedMsg;
  int recvCounter = 0;
  auto recvHandler = getRecvHandler<int>();
  EXPECT_CALL(*recvHandler, invoke)
      .WillOnce(Invoke([&receivedMsg, &recvCounter](int *msg) {
        receivedMsg = msg;
        recvCounter++;
      }));

  {
    SocketNAL<int> nal(getSocket(), recvSocket, encoder, recvHandler,
                       getErrHandler());
    while (waitFlag) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
  ASSERT_EQ(&msg, receivedMsg);
  ASSERT_EQ(1, recvCounter);
}

TEST(SocketNAL, ReportErrorFromMessageDecode) {
  Exception exception(rand());
  auto encoder = getEncoder<int>();
  EXPECT_CALL(*encoder, decode(_, _)).WillOnce(Throw(exception));

  bool waitFlag = true;
  auto recvSocket = getSocket();
  EXPECT_CALL(*recvSocket, recv(_, _, _))
      .WillOnce(Invoke([&waitFlag](const char *, int, int) {
        waitFlag = false;
        return 2;
      }))
      .WillRepeatedly(Return(0));

  int errCode = 0;
  int errCounter = 0;
  auto errHandler = getErrHandler();
  EXPECT_CALL(*errHandler, invoke)
      .WillOnce(Invoke([&errCode, &errCounter](const Exception &e) {
        errCode = e.getErrCode();
        errCounter++;
      }));
  {
    SocketNAL<int> nal(getSocket(), recvSocket, encoder, getRecvHandler<int>(),
                       errHandler);
    while (waitFlag) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  ASSERT_EQ(1, errCounter);
  ASSERT_EQ(exception.getErrCode(), errCode);
}

TEST(SocketNAL, ReportErrorFromReceive) {
  Exception exception(rand());
  auto encoder = getEncoder<int>();
  EXPECT_CALL(*encoder, decode(_, _)).Times(0);

  bool waitFlag = true;
  auto recvSocket = getSocket();
  EXPECT_CALL(*recvSocket, recv(_, _, _))
      .WillOnce(Invoke([&waitFlag, &exception](const char *, int, int) {
        waitFlag = false;
        throw exception;
        return 0;
      }))
      .WillRepeatedly(Return(0));

  int errCode = 0;
  int errCounter = 0;
  auto errHandler = getErrHandler();
  EXPECT_CALL(*errHandler, invoke)
      .WillOnce(Invoke([&errCounter, &errCode](const Exception &e) {
        errCounter++;
        errCode = e.getErrCode();
      }));

  {
    SocketNAL<int> nal(getSocket(), recvSocket, encoder, getRecvHandler<int>(),
                       errHandler);
    while (waitFlag) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  ASSERT_EQ(1, errCounter);
  ASSERT_EQ(exception.getErrCode(), errCode);
}

TEST(SocketNAL, HandleErrorCallbackFails) {
  auto encoder = getEncoder<int>();
  EXPECT_CALL(*encoder, decode(_, _)).WillOnce(Throw(Exception()));

  bool waitFlag = true;
  auto recvSocket = getSocket();
  EXPECT_CALL(*recvSocket, recv(_, _, _))
      .WillOnce(Invoke([&waitFlag](const char *, int, int) {
        waitFlag = false;
        return 2;
      }))
      .WillRepeatedly(Return(0));

  int callbackCount = 0;
  auto errHandler = getErrHandler();
  EXPECT_CALL(*errHandler, invoke)
      .WillOnce(Invoke([&callbackCount](const Exception &e) {
        callbackCount++;
        throw std::exception();
      }));

  {
    SocketNAL<int> nal(getSocket(), recvSocket, encoder, getRecvHandler<int>(),
                       errHandler);
    while (waitFlag) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  ASSERT_EQ(1, callbackCount);
}

TEST(SocketNAL, HandleRecvCallbackFails) {
  int msg;
  auto encoder = getEncoder<int>();
  EXPECT_CALL(*encoder, decode(_, _)).WillOnce(Return(&msg));

  bool waitFlag = true;
  auto recvSocket = getSocket();
  EXPECT_CALL(*recvSocket, recv(_, _, _))
      .WillOnce(Invoke([&waitFlag](const char *, int, int) {
        waitFlag = false;
        return 2;
      }))
      .WillRepeatedly(Return(0));

  int callbackCount = 0;
  auto recvHandler = getRecvHandler<int>();
  EXPECT_CALL(*recvHandler, invoke).WillOnce(Invoke([&callbackCount](int *) {
    callbackCount++;
    throw std::exception();
  }));

  {
    SocketNAL<int> nal(getSocket(), recvSocket, encoder, recvHandler,
                       getErrHandler());
    while (waitFlag) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  ASSERT_EQ(1, callbackCount);
}

TEST(SocketNAL, GetReceiveHandler) {
  auto recvHandler = getRecvHandler<int>();
  SocketNAL<int> nal(getSocket(), getSocket(), getEncoder<int>(), recvHandler,
                     getErrHandler());

  ASSERT_EQ(recvHandler, nal.getRecvHandler());
}

TEST(SocketNAL, GetErrorHandler) {
  auto errHandler = getErrHandler();
  SocketNAL<int> nal(getSocket(), getSocket(), getEncoder<int>(),
                     getRecvHandler<int>(), errHandler);

  ASSERT_EQ((void *)errHandler.get(), (void *)nal.getErrorHandler().get());
}