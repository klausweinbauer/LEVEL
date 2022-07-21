#pragma once

#include <ISocket.hpp>
#include <gmock/gmock.h>

class MSocket : public level::ISocket {
public:
  MOCK_METHOD(void, sendTo, (const char *buffer, int len, int flags),
              (override));
  MOCK_METHOD(int, recvFrom, (char *buffer, int len, int flags), (override));
  MOCK_METHOD(void, bindSocket, (), (override));
  MOCK_METHOD(void, close, (), (override));
};