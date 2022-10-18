#pragma once

#include <DENMWrapper.hpp>
#include <IDENBasicService.hpp>
#include <IDatabase.hpp>
#include <INetworkInterface.hpp>
#include <IPOTI.hpp>

namespace level::denm {

class DENBasicService : public IDENBasicService {

private:
  std::shared_ptr<INetworkInterface<DENM>> _nal;
  std::shared_ptr<IPOTI> _poti;
  std::shared_ptr<IDatabase<DENMWrapper>> _db;

public:
  virtual ~DENBasicService() {}

  DENBasicService(std::shared_ptr<INetworkInterface<DENM>> networkInterface,
                  std::shared_ptr<IPOTI> poti,
                  std::shared_ptr<IDatabase<DENMWrapper>> denmDatabase);

  virtual bool tryGetDENM(ActionId_t actionID, DENMWrapper *denm) override;

  virtual ActionId_t createDENM(EventType eventType) override;

  virtual void updateDENM(ActionId_t actionID) override;

  virtual void terminateDENM(ActionId_t actionID) override;
};

} // namespace level::denm