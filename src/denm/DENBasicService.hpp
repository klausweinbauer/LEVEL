#pragma once

#include <DENMWrapper.hpp>
#include <IDENBasicService.hpp>
#include <IDatabase.hpp>
#include <INetworkInterface.hpp>
#include <IPOTI.hpp>
#include <IValueConverter.hpp>
#include <mutex>

namespace level::denm {

class DENBasicService : public IDENBasicService {

private:
  std::shared_ptr<INetworkInterface<DENM>> _nal;
  std::shared_ptr<IPOTI> _poti;
  std::shared_ptr<IDatabase<DENMWrapper>> _db;
  std::shared_ptr<IValueConverter> _converter;
  int _nextSequenceNr;
  std::mutex _lock;
  DENBasicServiceConfig_t _config;

public:
  virtual ~DENBasicService() {}

  DENBasicService(std::shared_ptr<INetworkInterface<DENM>> networkInterface,
                  std::shared_ptr<IPOTI> poti,
                  std::shared_ptr<IDatabase<DENMWrapper>> denmDatabase,
                  std::shared_ptr<IValueConverter> valueConverter);

  virtual void configure(DENBasicServiceConfig_t config) override;

  virtual DENBasicServiceConfig_t getConfiguration() override;

  virtual bool tryGetDENM(ActionId_t actionID, DENMWrapper *denm) override;

  virtual ActionId_t createDENM(EventType *eventType = nullptr) override;

  virtual void updateDENM(ActionId_t actionID) override;

  virtual void terminateDENM(ActionId_t actionID) override;
};

} // namespace level::denm