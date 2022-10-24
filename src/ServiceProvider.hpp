#pragma once

#include <CABasicService.hpp>
#include <CAMRecvHandler.hpp>
#include <DENBasicService.hpp>
#include <DENMRecvHandler.hpp>
#include <DENMWrapper.hpp>
#include <Database.hpp>
#include <ErrorHandler.hpp>
#include <POTI.hpp>
#include <SocketNAL.hpp>
#include <Syscall.hpp>
#include <UDPSocket.hpp>
#include <VehicleFrequencyManager.hpp>
#include <functional>
#include <memory>

#define INSTANTIATION_FUNCTION(interface, implementation, name, ...)           \
  inline std::function<std::shared_ptr<interface>()> di##name##Provider =      \
      []() { return std::make_shared<implementation>(__VA_ARGS__); };

#define REG_TRANSIENT(interface, implementation, name, ...)                    \
  INSTANTIATION_FUNCTION(interface, implementation, name, __VA_ARGS__)         \
  inline std::shared_ptr<interface> get##name() { return di##name##Provider(); }

#define REG_SINGELTON(interface, implementation, name, ...)                    \
  INSTANTIATION_FUNCTION(interface, implementation, name, __VA_ARGS__)         \
  inline std::shared_ptr<interface> get##name() {                              \
    static std::shared_ptr<interface> instance = di##name##Provider();         \
    return instance;                                                           \
  }

namespace level::di {

using namespace level;

// Argument less
REG_TRANSIENT(IErrorHandler, ErrorHandler, ErrorHandler)
REG_TRANSIENT(IRecvHandler<CAM>, cam::CAMRecvHandler, CAMRecvHandler)
REG_TRANSIENT(IRecvHandler<DENM>, denm::DENMRecvHandler, DENMRecvHandler)
REG_SINGELTON(ISyscall, Syscall, Syscall)
REG_SINGELTON(IPOTI, POTI, POTI)
REG_SINGELTON(IValueConverter, ValueConverter, ValueConverter)
REG_SINGELTON(IEncoder<CAM>, cam::CAMDEREncoder, CAMEncoder)
REG_SINGELTON(IEncoder<DENM>, denm::DENMDEREncoder, DENMEncoder)
REG_SINGELTON(IDatabase<cam::CAMWrapper>, Database<cam::CAMWrapper>,
              CAMDatabase)
REG_SINGELTON(IDatabase<denm::DENMWrapper>, Database<denm::DENMWrapper>,
              DENMDatabase)

// With Arguments
REG_TRANSIENT(cam::IFrequencyManager, cam::VehicleFrequencyManager,
              VehicleFrequencyManager, getValueConverter(), getPOTI())
REG_TRANSIENT(ISocket, UDPSocket, Socket, 5100, getSyscall())
REG_SINGELTON(INetworkInterface<CAM>, SocketNAL<CAM>, CAMNetwork, getSocket(),
              getSocket(), getCAMEncoder(), getCAMRecvHandler(),
              getErrorHandler())
REG_SINGELTON(INetworkInterface<DENM>, SocketNAL<DENM>, DENMNetwork,
              getSocket(), getSocket(), getDENMEncoder(), getDENMRecvHandler(),
              getErrorHandler())
REG_SINGELTON(cam::ICABasicService, cam::CABasicService, CABasicService,
              getCAMNetwork(), getValueConverter(),
              getVehicleFrequencyManager(), getPOTI(), getCAMDatabase())
REG_SINGELTON(denm::IDENBasicService, denm::DENBasicService, DENBasicService,
              getDENMNetwork(), getPOTI(), getDENMDatabase(),
              getValueConverter())

} // namespace level::di