// Copyright 2022 Klaus Weinbauer
//
// This file is part of LEVEL.
//
// LEVEL is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// LEVEL is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LEVEL. If not, see <https://www.gnu.org/licenses/>.

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

#ifdef __cplusplus
}
#endif