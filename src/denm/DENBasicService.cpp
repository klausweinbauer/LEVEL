#include <DENBasicService.hpp>
#include <DENMIndexer.hpp>
#include <QRYLatestMsg.hpp>

namespace level::denm {

DENBasicService::DENBasicService(
    std::shared_ptr<INetworkInterface<DENM>> networkInterface,
    std::shared_ptr<IPOTI> poti,
    std::shared_ptr<IDatabase<DENMWrapper>> denmDatabase)
    : _nal(networkInterface), _poti(poti), _db(denmDatabase) {

  // Add indexer to denm database
  _db->addIndexer(std::make_unique<IDXDENMIndex>());
  _db->addIndexer(std::make_unique<IDXDENMStationID>());
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

ActionId_t DENBasicService::createDENM(EventType eventType) { return {}; }

void DENBasicService::updateDENM(ActionId_t actionID) {}

void DENBasicService::terminateDENM(ActionId_t actionID) {}

} // namespace level::denm