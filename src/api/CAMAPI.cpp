#include <Exception.hpp>
#include <ServiceProvider.hpp>
#include <level.h>

namespace level {

int configureCABasicService(CABasicServiceConfig_t config) {
  di::getCABasicService()->configure(config);
  return 0;
}

CABasicServiceConfig_t getCABasicServiceConfig() {
  return di::getCABasicService()->getConfiguration();
}

int getCAMBasicContainer(int stationID, CAMBasicContainerData_t *container) {
  // TODO Implement
  return 0;
}

int getCAMBasicVehicleContainerHighFrequency(
    int stationID, CAMBasicVehicleContainerHighFrequencyData_t *container) {
  // TODO Implement
  return 0;
}

} // namespace level