#pragma once

#include <DBElement.hpp>
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

template <typename T> class MDBElement : public DBElement<T> {
public:
  MDBElement() : DBElement<T>(0){};

  MOCK_METHOD(DBView<T>, getView, (), (override));
  MOCK_METHOD(T &, data, (), (override, const));
  MOCK_METHOD(void, setData, (T * data), (override));
  MOCK_METHOD(DBElementStatus *, status, (), (override, const));
  MOCK_METHOD(void, setStatus, (DBElementStatus * status), (override));
  MOCK_METHOD(void, unlock, (), (override));
  MOCK_METHOD(void, lock, (), (override));
  MOCK_METHOD(const std::thread::id &, holdingThread, (), (override, const));
  MOCK_METHOD(unsigned int, getIndex, (), (override, const));
};