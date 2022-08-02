#include <level.h>

namespace level {

CAMVDPUnit::CAMVDPUnit() {}

CAMVDPUnit::CAMVDPUnit(int heading, int speed, int driveDirection,
                       int acceleration, int curvature, int yawRate)
    : heading(heading), speed(speed), driveDirection(driveDirection),
      acceleration(acceleration), curvature(curvature), yawRate(yawRate) {}

void VDP_t::setHeading(float heading) {
  if (heading < 0) {
    heading += ((int)(heading / -360.0) + 1) * 360.0;
  }
  _heading = ROUND_TO_INT(heading * 10.0) % 3600;
}

void VDP_t::setSpeed(float speed) {
  _speed = ROUND_TO_INT(speed * 100.0);
  if (_speed < 0)
    _speed = 0;
  else if (_speed > 16382)
    _speed = 16382;
}

void VDP_t::setDriveDirection(DriveDirectionType direction) {
  if (direction == DriveDirection_Forward ||
      direction == DirveDirection_Backward) {
    _driveDirection = direction;
  } else {
    _driveDirection = DriveDirection_Unavailable;
  }
}

void VDP_t::setAcceleration(float acceleration) {
  _acceleration = acceleration * 10;
  if (_acceleration < -160)
    _acceleration = -160;
  else if (_acceleration > 160)
    _acceleration = 160;
}

void VDP_t::setCurvature(float radius) {
  if (radius == 0) {
    _curvature = 0;
  } else {
    _curvature = 10000.0 / radius;
    if (_curvature < -1023)
      _curvature = -1023;
    else if (_curvature > 1022)
      _curvature = 1022;
  }
}

void VDP_t::setYawRate(float yawRate) {
  _yawRate = yawRate * 100;
  if (_yawRate > 32766)
    _yawRate = 32766;
  else if (_yawRate < -32766)
    _yawRate = -32766;
}

CAMVDPUnit VDP_t::getCAMDataUnit() {
  return CAMVDPUnit(_heading, _speed, _driveDirection, _acceleration,
                    _curvature, _yawRate);
}

} // namespace level