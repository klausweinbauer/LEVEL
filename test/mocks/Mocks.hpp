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

#pragma once

#include <CAMWrapper.hpp>
#include <DBElement.hpp>
#include <ICABasicService.hpp>
#include <IDENBasicService.hpp>
#include <IDXParameter.hpp>
#include <IDatabase.hpp>
#include <IEncoder.hpp>
#include <IFrequencyManager.hpp>
#include <IIndexer.hpp>
#include <INetworkInterface.hpp>
#include <IPOTI.hpp>
#include <IQRYParameter.hpp>
#include <IQuery.hpp>
#include <ISocket.hpp>
#include <ISyscall.hpp>
#include <IValueConverter.hpp>
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
class MIDXParameter : public IDXParameter<TData, TParameter> {
public:
  MIDXParameter() {}

  MOCK_METHOD(TParameter, getValue, (const TData &), (override, const));
  MOCK_METHOD(std::vector<unsigned int>, getByParameter,
              (std::shared_ptr<QRYParameter<TParameter>>), (override, const));
};

template <typename T> class MNetworkInterface : public INetworkInterface<T> {
public:
  virtual ~MNetworkInterface() {}

  MOCK_METHOD(bool, send, (const T *), (override));
  MOCK_METHOD(std::shared_ptr<IRecvHandler<T>>, getRecvHandler, (), (override));
  MOCK_METHOD(std::shared_ptr<IErrorHandler>, getErrorHandler, (), (override));
};

class MSyscall : public ISyscall {
public:
  virtual ~MSyscall() {}

  MOCK_METHOD(int, sysPoll, (PollFD *, nfds_l, int), (override));
  MOCK_METHOD(int, sysSocket, (SockDomain, SockType, Protocol), (override));
  MOCK_METHOD(int, sysSendTo,
              (int, const void *, size_t, int, const SockAddr *, SockLen),
              (override));
  MOCK_METHOD(int, sysSetSockOpt,
              (int, ProtocolLevel, SocketOption, const void *, SockLen),
              (override));
  MOCK_METHOD(int, sysClose, (int), (override));
  MOCK_METHOD(int, sysBind, (int, const SockAddr *, SockLen), (override));
  MOCK_METHOD(int, sysRecvFrom,
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

  MOCK_METHOD(int, siToITSHeading, (float), (override));
  MOCK_METHOD(float, itsToSIHeading, (int), (override));
  MOCK_METHOD(int, siToITSSpeed, (float), (override));
  MOCK_METHOD(float, itsToSISpeed, (int), (override));
  MOCK_METHOD(int, siToITSLongitudinalAcceleration, (float), (override));
  MOCK_METHOD(float, itsToSILongitudinalAcceleration, (int), (override));
  MOCK_METHOD(int, siToITSCurvature, (float), (override));
  MOCK_METHOD(float, itsToSICurvature, (int), (override));
  MOCK_METHOD(int, siToITSYawRate, (float), (override));
  MOCK_METHOD(float, itsToSIYawRate, (int), (override));
  MOCK_METHOD(int, timestampToDeltaTime, (unsigned long long int), (override));
  MOCK_METHOD(int, siToITSLongitude, (float), (override));
  MOCK_METHOD(float, itsToSILongitude, (int), (override));
  MOCK_METHOD(int, siToITSLatitude, (float), (override));
  MOCK_METHOD(float, itsToSILatitude, (int), (override));
  MOCK_METHOD(float, distance, (float, float, float, float), (override));
  MOCK_METHOD(void, siToITSTimestamp,
              (unsigned long long int, TimestampIts_t &), (override));
  MOCK_METHOD(unsigned long long int, itsToSITimestamp,
              (const TimestampIts_t &), (override));
};

class MCABasicService : public cam::ICABasicService {
public:
  virtual ~MCABasicService() {}

  MOCK_METHOD(void, configure, (CABasicServiceConfig), (override));
  MOCK_METHOD(CABasicServiceConfig, getConfiguration, (), (override));
  MOCK_METHOD(float, getCAMGenerationFrequency, (), (override));
  MOCK_METHOD(cam::CAMWrapper, cam, (), (override));
  MOCK_METHOD(bool, tryGetCAM, (unsigned int, cam::CAMWrapper *), (override));
  MOCK_METHOD(void, setHeading, (float), (override));
  MOCK_METHOD(void, setSpeed, (float), (override));
  MOCK_METHOD(void, setDriveDirection, (DriveDirectionType), (override));
  MOCK_METHOD(void, setAcceleration, (float), (override));
  MOCK_METHOD(void, setCurvature, (float), (override));
  MOCK_METHOD(void, setYawRate, (float), (override));
};

class MPOTI : public IPOTI {
public:
  virtual ~MPOTI() {}

  MOCK_METHOD(unsigned long long int, now, (), (const, override));
};

class MFrequencyManager : public cam::IFrequencyManager {
public:
  virtual ~MFrequencyManager() {}

  MOCK_METHOD(int, getTCheckCAMGen, (), (override));
  MOCK_METHOD(int, getTCAMGen, (), (override));
  MOCK_METHOD(bool, generateCAM, (cam::CAMWrapper &), (override));
  MOCK_METHOD(bool, includeLFC, (), (override));
  MOCK_METHOD(void, notifyCAMGeneration, (cam::CAMWrapper &), (override));
};

template <typename T> class MRecvHandler : public IRecvHandler<T> {
public:
  virtual ~MRecvHandler() {}

  MOCK_METHOD(void, registerCallback, (std::function<void(T *, bool *)>),
              (override));
  MOCK_METHOD(void, unregisterCallbacks, (), (override));
  MOCK_METHOD(int, callbackCount, (), (override));
  MOCK_METHOD(void, invoke, (T *), (override));
};

class MErrorHandler : public IErrorHandler {
public:
  virtual ~MErrorHandler() {}

  MOCK_METHOD(void, registerCallback, (std::function<void(const Exception &)>),
              (override));
  MOCK_METHOD(void, unregisterCallbacks, (), (override));
  MOCK_METHOD(int, callbackCount, (), (override));
  MOCK_METHOD(void, invoke, (const Exception &), (override));
};

class MDENBasicService : public denm::IDENBasicService {
public:
  virtual ~MDENBasicService() {}

  MOCK_METHOD(void, configure, (DENBasicServiceConfig_t), (override));
  MOCK_METHOD(DENBasicServiceConfig_t, getConfiguration, (), (override));
  MOCK_METHOD(bool, tryGetDENM, (ActionId_t, denm::DENMWrapper *), (override));
  MOCK_METHOD(ActionId_t, createDENM, (EventType * eventType), (override));
  MOCK_METHOD(void, updateDENM, (ActionId_t), (override));
  MOCK_METHOD(void, terminateDENM, (ActionId_t), (override));
};