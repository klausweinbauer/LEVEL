#include <ValueConverter.hpp>

#define ROUND(x) (int)(x + 0.5)

namespace level {

int ValueConverter::siToITSHeading(float heading) {
  if (heading < 0) {
    heading += ((int)(heading / -360.0) + 1) * 360.0;
  }
  int itsValue = ROUND(heading * 10.0) % 3600;
  return itsValue;
}

float ValueConverter::itsToSIHeading(int heading) { return heading / 10.0; }

int ValueConverter::siToITSSpeed(float speed) {
  int itsValue = ROUND(speed * 100.0);
  if (itsValue < 0)
    itsValue = 0;
  else if (itsValue > 16382)
    itsValue = 16382;
  return itsValue;
}

float ValueConverter::itsToSISpeed(int speed) { return speed / 100.0; }

int ValueConverter::siToITSLongitudinalAcceleration(float acceleration) {
  int itsValue = acceleration * 10;
  if (itsValue < -160)
    itsValue = -160;
  else if (itsValue > 160)
    itsValue = 160;
  return itsValue;
}

float ValueConverter::itsToSILongitudinalAcceleration(int acceleration) {
  return acceleration / 10.0;
}

int ValueConverter::siToITSCurvature(float radius) {
  int itsValue;
  if (radius == 0) {
    itsValue = 0;
  } else {
    itsValue = 10000.0 / radius;
    if (itsValue < -1023)
      itsValue = -1023;
    else if (itsValue > 1022)
      itsValue = 1022;
  }
  return itsValue;
}

float ValueConverter::itsToSICurvature(int curvature) {
  if (curvature == 0) {
    return 0;
  } else {
    return 10000.0 / curvature;
  }
}

int ValueConverter::siToITSYawRate(float yawRate) {
  int itsValue = yawRate * 100;
  if (itsValue > 32766)
    itsValue = 32766;
  else if (itsValue < -32766)
    itsValue = -32766;
  return itsValue;
}

float ValueConverter::itsToSIYawRate(int yawRate) { return yawRate / 100.0; }

int ValueConverter::timestampToDeltaTime(unsigned long long int timestamp) {
  return timestamp % 65536;
}

} // namespace level