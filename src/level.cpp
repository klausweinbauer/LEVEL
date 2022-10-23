#include <ServiceProvider.hpp>
#include <level.h>

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

#ifndef PROJECT_VERSION
#define PROJECT_VERSION "unknown"
#endif

#ifdef __cplusplus
namespace level {
#endif

const char *getVersion() { return TO_STRING(PROJECT_VERSION); }

void setHeading(float heading) { di::getCABasicService()->setHeading(heading); }

void setSpeed(float speed) { di::getCABasicService()->setSpeed(speed); }

void setDriveDirection(DriveDirectionType direction) {
  di::getCABasicService()->setDriveDirection(direction);
}

void setAcceleration(float acceleration) {
  di::getCABasicService()->setAcceleration(acceleration);
}

void setCurvature(float radius) {
  di::getCABasicService()->setCurvature(radius);
}

void setYawRate(float yawRate) { di::getCABasicService()->setYawRate(yawRate); }

int configureCABasicService(CABasicServiceConfig_t config) {
  di::getCABasicService()->configure(config);
  return 0;
}

CABasicServiceConfig_t getCABasicServiceConfig() {
  return di::getCABasicService()->getConfiguration();
}

int getCAMBasicContainer(int stationID, CAMBasicContainer_t *container) {
  cam::CAMWrapper cam;
  if (di::getCABasicService()->tryGetCAM(stationID, &cam)) {
    return cam.getBasicContainer(container);
  } else {
    return ERR_MSG_NOT_FOUND;
  }
}

int getCAMBasicVehicleContainerHF(int stationID,
                                  CAMBasicVehicleContainerHF_t *container) {
  cam::CAMWrapper cam;
  if (di::getCABasicService()->tryGetCAM(stationID, &cam)) {
    return cam.getBasicVehicleContainerHighFrequency(container);
  } else {
    return ERR_MSG_NOT_FOUND;
  }
}

void clearErrMsg() { Exception::clearErrMsg(); }

void getErrMsg(int err, char *buffer, int size) {
  Exception::getErrMsg(err, buffer, size);
}

void getLastErrMsg(char *buffer, int size, int *actualSize) {
  Exception::getLastErrMsg(buffer, size, actualSize);
}

int configureCABasicService(DENBasicServiceConfig_t config) {
  di::getDENBasicService()->configure(config);
  return 0;
}

DENBasicServiceConfig_t getDENBasicServiceConfig() {
  return di::getDENBasicService()->getConfiguration();
}

#ifdef __cplusplus
}
#endif