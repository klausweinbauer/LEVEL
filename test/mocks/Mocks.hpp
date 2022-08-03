#pragma once

#include <DBElement.hpp>
#include <IDatabase.hpp>
#include <IEncoder.hpp>
#include <IIndexer.hpp>
#include <IQRYParameter.hpp>
#include <IQuery.hpp>
#include <ISocket.hpp>
#include <ISyscall.hpp>
#include <IValueConverter.hpp>
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
  MOCK_METHOD(bool, send, (const char *, int), (override));
  MOCK_METHOD(int, recv, (char *, int, int), (override));
  MOCK_METHOD(int, read, (char *, int, const bool *const), (override));
};

class MQuery : public IQuery {
public:
};

template <typename T> class MIndexer : public IIndexer<T> {
public:
  MOCK_METHOD(bool, supportsQuery, (std::shared_ptr<IQuery>),
              (override, const));
  MOCK_METHOD(std::vector<unsigned int>, getIndexList,
              (std::shared_ptr<IQuery>), (override, const));
  MOCK_METHOD(void, addData, (const T &, unsigned int), (override));
  MOCK_METHOD(void, updateData, (const T &, unsigned int), (override));
  MOCK_METHOD(void, removeData, (const T &, unsigned int), (override));
  MOCK_METHOD(bool, isValid, (const T &, unsigned int), (override, const));
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
  MOCK_METHOD(void, addIndexer, (std::unique_ptr<IIndexer<T>>), (override));
  MOCK_METHOD(unsigned int, count, (), (override));
  MOCK_METHOD(DBView<T>, insert, (T), (override));
  MOCK_METHOD(DBView<T>, insert, (std::unique_ptr<T>), (override));
  MOCK_METHOD(std::vector<DBView<T>>, get, (std::shared_ptr<IQuery>),
              (override));
  MOCK_METHOD(bool, remove, (DBView<T> &), (override));
  MOCK_METHOD(bool, remove, (DBView<T> &&), (override));
};

template <typename TData, typename TParameter>
class MParameterIndexer : public ParameterIndexer<TData, TParameter> {
public:
  MParameterIndexer() {}

  MOCK_METHOD(TParameter, getValue, (const TData &), (override, const));
  MOCK_METHOD(std::vector<unsigned int>, getByParameter,
              (std::shared_ptr<QRYParameter<TParameter>>), (override, const));
};

class MSyscall : public ISyscall {
public:
  virtual ~MSyscall() {}

  MOCK_METHOD(int, sysPoll, (PollFD *, nfds_l, int), (override));
  MOCK_METHOD(int, sysSocket, (SockDomain, SockType, Protocol), (override));
  MOCK_METHOD(ssize_t, sysSendTo,
              (int, const void *, size_t, int, const SockAddr *, SockLen),
              (override));
  MOCK_METHOD(int, sysSetSockOpt,
              (int, ProtocolLevel, SocketOption, const void *, SockLen),
              (override));
  MOCK_METHOD(int, sysClose, (int), (override));
  MOCK_METHOD(int, sysBind, (int, const SockAddr *, SockLen), (override));
  MOCK_METHOD(ssize_t, sysRecvFrom,
              (int, void *, size_t, int, SockAddr *, SockLen *), (override));
};

template <typename T> class MEncoder : public IEncoder<T> {
public:
  virtual ~MEncoder() {}

  MOCK_METHOD(std::vector<char>, encode, (const T *), (override));
  MOCK_METHOD(T *, decode, (const std::vector<char>), (override));
  MOCK_METHOD(T *, decode, (const char *, int), (override));
};

class MValueConverter : public IValueConverter {
public:
  virtual ~MValueConverter() {}

  MOCK_METHOD(int, siToITSHeading, (float heading), (override));
  MOCK_METHOD(float, itsToSIHeading, (int heading), (override));
  MOCK_METHOD(int, siToITSSpeed, (float speed), (override));
  MOCK_METHOD(float, itsToSISpeed, (int speed), (override));
  MOCK_METHOD(int, siToITSLongitudinalAcceleration, (float acceleration),
              (override));
  MOCK_METHOD(float, itsToSILongitudinalAcceleration, (int acceleration),
              (override));
  MOCK_METHOD(int, siToITSCurvature, (float curvature), (override));
  MOCK_METHOD(float, itsToSICurvature, (int curvature), (override));
  MOCK_METHOD(int, siToITSYawRate, (float yawRate), (override));
  MOCK_METHOD(float, itsToSIYawRate, (int yawRate), (override));
};