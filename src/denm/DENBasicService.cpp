#include <DENBasicService.hpp>
#include <DENMIndexer.hpp>
#include <QRYLatestMsg.hpp>

#define SAFE_ASSIGN(containerType, containerProperty, property, value)         \
  if (value != nullptr) {                                                      \
    if (containerProperty == nullptr) {                                        \
      containerProperty = new containerType();                                 \
    }                                                                          \
    containerProperty->property = *value;                                      \
  }

namespace level::denm {

DENBasicService::DENBasicService(
    std::shared_ptr<INetworkInterface<DENM>> networkInterface,
    std::shared_ptr<IPOTI> poti,
    std::shared_ptr<IDatabase<DENMWrapper>> denmDatabase,
    std::shared_ptr<IValueConverter> valueConverter)
    : _nal(networkInterface), _poti(poti), _db(denmDatabase),
      _converter(valueConverter), _nextSequenceNr(1) {

  // Add indexer to denm database
  _db->addIndexer(std::make_unique<IDXDENMIndex>());
  _db->addIndexer(std::make_unique<IDXDENMStationID>());

  // Setup default config
  _config.stationID = rand();
  _config.stationType = StationType::StationType_PassengerCar;
  configure(_config);
}

void DENBasicService::configure(DENBasicServiceConfig_t config) {
  std::lock_guard<std::mutex> _guard(_lock);
  _config = config;
}

DENBasicServiceConfig_t DENBasicService::getConfiguration() {
  std::lock_guard<std::mutex> _guard(_lock);
  return _config;
}

bool DENBasicService::tryGetDENM(ActionId_t actionID, DENMWrapper *denm) {
  auto views = _db->get(QRYLatestMsg::byId(actionID.stationID));

  for (auto &&view : views) {
    if (view->getMC()->actionID.sequenceNumber == actionID.sequenceNumber) {
      *denm = *view;
      return true;
    }
  }

  return false;
}

ActionId_t DENBasicService::createDENM(EventType *eventType) {
  // TODO Add support for validity duration

  // Assign unused action id
  _lock.lock();
  ActionId_t actionId;
  actionId.stationID = _config.stationID;
  actionId.sequenceNumber = _nextSequenceNr++;
  _lock.unlock();
  DENMWrapper denm(actionId);

  // TODO Add support for transmission interval and repetition

  // Set provided fields
  SAFE_ASSIGN(SituationContainer, denm->denm.situation, eventType.causeCode,
              (CauseCodeType_t *)eventType)

  // Set referenceTime
  denm->denm.management.detectionTime = _converter->siToITSTimestamp(
      _poti->now()); // TODO fix converter to accept reference

  // Pass to NAL

  _db->insert(denm);

  return actionId;
}

void DENBasicService::updateDENM(ActionId_t actionID) {}

void DENBasicService::terminateDENM(ActionId_t actionID) {}

} // namespace level::denm