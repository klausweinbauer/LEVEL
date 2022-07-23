#pragma once

#include <IIndexer.hpp>
#include <IQuery.hpp>
#include <ISocket.hpp>
#include <gmock/gmock.h>
#include <vector>

using namespace level;

class MSocket : public ISocket {
public:
  MOCK_METHOD(void, send, (const char *buffer, int len, int flags), (override));
  MOCK_METHOD(int, recv, (char *buffer, int len, int flags), (override));
  MOCK_METHOD(void, close, (), (override));
};

class MQuery : public IQuery {
public:
  MOCK_METHOD(int, getQueryType, (), (override, const));
};

template <typename T> class MIndexer : public IIndexer<T> {
public:
  MOCK_METHOD(int, getQueryType, (), (override, const));
  MOCK_METHOD(std::vector<unsigned int>, getIndexList, (const IQuery &query),
              (override));
  MOCK_METHOD(void, valueChanged, (const T *const entry, unsigned int index),
              (override));
};