#include <DENBasicService.hpp>

namespace level::denm {

bool DENBasicService::tryGetDENM(ActionID_t actionID, DENMWrapper *denm) {
  return false;
}

ActionID_t DENBasicService::createDENM(EventType eventType) { return {}; }

void DENBasicService::updateDENM(ActionID_t actionID) {}

void DENBasicService::terminateDENM(ActionID_t actionID) {}

} // namespace level::denm