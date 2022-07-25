#pragma once

#include <DBElement.hpp>
#include <IDatabase.hpp>
#include <IIndexer.hpp>
#include <IQRYParameter.hpp>
#include <IQuery.hpp>
#include <ISocket.hpp>
#include <ParameterIndexer.hpp>
#include <gmock/gmock.h>
#include <vector>

using namespace level;

struct Dummy {
  int _value;

  Dummy() {}
  Dummy(int value) : _value(value) {}
};

class MSocket : public ISocket {
public:
  MOCK_METHOD(void, send, (const char *buffer, int len, int flags), (override));
  MOCK_METHOD(int, recv, (char *buffer, int len, int flags), (override));
  MOCK_METHOD(void, close, (), (override));
};

class MQuery : public IQuery {
public:
};

template <typename T> class MIndexer : public IIndexer<T> {
public:
  MOCK_METHOD(bool, supportsQuery, (std::shared_ptr<IQuery>));
  MOCK_METHOD(std::vector<unsigned int>, getIndexList,
              (std::shared_ptr<IQuery>));
  MOCK_METHOD(void, addData, (const T &, unsigned int), (override));
  MOCK_METHOD(void, updateData, (const T &, unsigned int), (override));
  MOCK_METHOD(void, removeData, (const T &, unsigned int), (override));
};

template <typename T> class MDBElement : public DBElement<T> {
public:
  MOCK_METHOD(T &, data, (), (override, const));
  MOCK_METHOD(void, setData, (std::unique_ptr<T>), (override));
  MOCK_METHOD(void, unlock, (), (override));
  MOCK_METHOD(void, lock, (), (override));
  MOCK_METHOD(void, clear, (), (override));
  MOCK_METHOD(const std::thread::id &, holdingThread, (), (override, const));
  MOCK_METHOD(unsigned int, index, (), (override, const));
};

template <typename T> class MDatabase : public IDatabase<T> {
public:
  MOCK_METHOD(void, addIndexer, (std::shared_ptr<IIndexer<T>>), (override));
  MOCK_METHOD(unsigned int, count, (), (override));
  MOCK_METHOD(DBView<T>, insert, (T), (override));
  MOCK_METHOD(DBView<T>, insert, (std::unique_ptr<T>), (override));
  MOCK_METHOD(std::vector<DBView<T>>, get, (std::shared_ptr<IQuery>),
              (override));
  MOCK_METHOD(bool, remove, (DBView<T> &), (override));
  MOCK_METHOD(bool, remove, (DBView<T> &&), (override));
  MOCK_METHOD(bool, remove, (unsigned int), (override));
};

template <typename TData, typename TParameter>
class MParameterIndexer : public ParameterIndexer<TData, TParameter> {
public:
  MParameterIndexer() {}

  MOCK_METHOD(TParameter, getValue, (const TData &), (override));
  MOCK_METHOD(std::vector<unsigned int>, getByParameter,
              (std::shared_ptr<QRYParameter<TParameter>>), (override));
};