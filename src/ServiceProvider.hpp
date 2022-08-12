#pragma once

#include <CABasicService.hpp>
#include <CAMRecvHandler.hpp>
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

using namespace level::cam;

// Argument less
REG_TRANSIENT(IErrorHandler, ErrorHandler, ErrorHandler)
REG_TRANSIENT(IRecvHandler<CAM>, CAMRecvHandler, CAMRecvHandler)
REG_SINGELTON(ISyscall, Syscall, Syscall)
REG_SINGELTON(IPOTI, POTI, POTI)
REG_SINGELTON(VDP_t, VDP_t, VDP)
REG_SINGELTON(IValueConverter, ValueConverter, ValueConverter)
REG_SINGELTON(IEncoder<CAM>, CAMDEREncoder, CAMEncoder)
REG_SINGELTON(IDatabase<CAMWrapper>, Database<CAMWrapper>, CAMDatabase)

// With Arguments
REG_TRANSIENT(IFrequencyManager, VehicleFrequencyManager,
              VehicleFrequencyManager, getValueConverter(), getPOTI())
REG_TRANSIENT(ISocket, UDPSocket, Socket, 5100, getSyscall())
REG_SINGELTON(INetworkInterface<CAM>, SocketNAL<CAM>, CAMNetwork, getSocket(),
              getSocket(), getCAMEncoder(), getCAMRecvHandler(),
              getErrorHandler())
REG_SINGELTON(ICABasicService, CABasicService, CABasicService, getCAMNetwork(),
              getValueConverter(), getVehicleFrequencyManager(), getPOTI(),
              getCAMDatabase())

} // namespace level::di