#include <CABasicService.hpp>
#include <ServiceProvider.hpp>
#include <iostream>
#include <level.h>

namespace level {

using namespace level::di;

VDP_t *getVDP() { return di::getVDP().get(); }

void VDP_t::setHeading(float heading) {
  auto p = di::getCABasicService().get();
  p->setHeading(heading);
}

void VDP_t::setSpeed(float speed) {
  auto p = di::getCABasicService().get();
  p->setSpeed(speed);
}

void VDP_t::setDriveDirection(DriveDirectionType direction) {
  di::getCABasicService()->setDriveDirection(direction);
}

void VDP_t::setAcceleration(float acceleration) {
  di::getCABasicService()->setAcceleration(acceleration);
}

void VDP_t::setCurvature(float radius) {
  di::getCABasicService()->setCurvature(radius);
}

void VDP_t::setYawRate(float yawRate) {
  di::getCABasicService()->setYawRate(yawRate);
}

} // namespace level