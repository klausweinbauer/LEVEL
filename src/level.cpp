#include <level.h>
#include <ServiceProvider.hpp>

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

#ifndef PROJECT_VERSION
#define PROJECT_VERSION "unknown"
#endif

#ifdef __cplusplus
namespace level {
#endif

const char *getVersion() { return TO_STRING(PROJECT_VERSION); }

void VDP_setHeading(float heading) {
  di::getCABasicService()->setHeading(heading);
}

void VDP_setSpeed(float speed) {
  di::getCABasicService()->setSpeed(speed);
}

void VDP_setDriveDirection(DriveDirectionType direction) {
  di::getCABasicService()->setDriveDirection(direction);
}

void VDP_setAcceleration(float acceleration) {
  di::getCABasicService()->setAcceleration(acceleration);
}

void VDP_setCurvature(float radius) {
  di::getCABasicService()->setCurvature(radius);
}

void VDP_setYawRate(float yawRate) {
  di::getCABasicService()->setYawRate(yawRate);
}

int configureCABasicService(CABasicServiceConfig_t config) {
  di::getCABasicService()->configure(config);
  return 0;
}

CABasicServiceConfig_t getCABasicServiceConfig() {
  return di::getCABasicService()->getConfiguration();
}

int getCAMBasicContainer(int stationID, CAMBasicContainerData_t *container) {
  cam::CAMWrapper cam;
  if (di::getCABasicService()->tryGetCAM(stationID, &cam)) {
    return cam.getBasicContainer(container);
  } else {
    return ERR_MSG_NOT_FOUND;
  }
}

int getCAMBasicVehicleContainerHighFrequency(
    int stationID, CAMBasicVehicleContainerHighFrequencyData_t *container) {
  cam::CAMWrapper cam;
  if (di::getCABasicService()->tryGetCAM(stationID, &cam)) {
    return cam.getBasicVehicleContainerHighFrequency(container);
  } else {
    return ERR_MSG_NOT_FOUND;
  }
}

#ifdef __cplusplus
}
#endif