#pragma once

#include <ISocket.hpp>
#include <gmock/gmock.h>

class MSocket : public level::ISocket {
public:
  MOCK_METHOD(void, send, (const char *buffer, int len, int flags), (override));
  MOCK_METHOD(int, recv, (char *buffer, int len, int flags), (override));
  MOCK_METHOD(void, close, (), (override));
};