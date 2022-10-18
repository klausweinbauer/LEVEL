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

  virtual bool tryGetDENM(ActionId_t actionID, DENMWrapper *denm) = 0;

  // TODO look at application requirements for a valid DENM generation
  virtual ActionId_t createDENM(EventType eventType) = 0;

  virtual void updateDENM(ActionId_t actionID) = 0;

  virtual void terminateDENM(ActionId_t actionID) = 0;
};

} // namespace level::denm