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

} // namespace level