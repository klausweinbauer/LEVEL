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

int getCAMBasicContainer(int stationID, CAMBasicContainerData_t *container) {}

int getCAMBasicVehicleContainerHighFrequency(
    int stationID, CAMBasicVehicleContainerHighFrequencyData_t *container) {}

} // namespace level