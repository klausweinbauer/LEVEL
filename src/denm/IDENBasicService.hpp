#pragma once

#include <DENMWrapper.hpp>

namespace level::denm {

class IDENBasicService {

  // TODO transmission is initiated by an ITS-S application
  // TODO termination by ITS-S application or by facility layer when expiry time
  // is reached
  // TODO transmission my be repeated
  // TODO forwarding
  // TODO receiving and notifying application layer

public:
  virtual ~IDENBasicService() {}

  /**
   * @brief Configure the decentralized environmental notification basic
   * service.
   *
   * @param config Configuration.
   */
  virtual void configure(DENBasicServiceConfig_t config) = 0;

  /**
   * @brief Returns the current decentralized environmental notification basic
   * service configuration.
   *
   * @return DENBasicServiceConfig Current configuration.
   */
  virtual DENBasicServiceConfig_t getConfiguration() = 0;

  virtual bool tryGetDENM(ActionId_t actionID, DENMWrapper *denm) = 0;

  virtual ActionId_t createDENM(EventType *eventType = nullptr) = 0;

  virtual void updateDENM(ActionId_t actionID) = 0;

  virtual void terminateDENM(ActionId_t actionID) = 0;
};

} // namespace level::denm