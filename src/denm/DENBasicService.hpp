#pragma once

#include <IDENBasicService.hpp>

namespace level::denm {

class DENBasicService : public IDENBasicService {

public:
  virtual ~DENBasicService() {}

  virtual bool tryGetDENM(ActionID_t actionID, DENMWrapper *denm) override;

  virtual ActionID_t createDENM(EventType eventType) override;

  virtual void updateDENM(ActionID_t actionID) override;

  virtual void terminateDENM(ActionID_t actionID) override;
};

} // namespace level::denm