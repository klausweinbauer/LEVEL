#include <Exception.hpp>
#include <ServiceProvider.hpp>
#include <level.h>

namespace level {

int configureCABasicService(CABasicServiceConfig_t config) {
  try {
    di::getCABasicService()->configure(config);
  } catch (const Exception &e) {
    return e.getErrCode();
  }
}

CABasicServiceConfig_t getCABasicServiceConfig() {
  return di::getCABasicService()->getConfiguration();
}

} // namespace level