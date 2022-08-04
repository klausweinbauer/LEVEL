#pragma once

namespace level {

class IValueConverter {
public:
  virtual ~IValueConverter() {}

  virtual int siToITSHeading(float heading) = 0;
  virtual float itsToSIHeading(int heading) = 0;

  virtual int siToITSSpeed(float speed) = 0;
  virtual float itsToSISpeed(int speed) = 0;

  virtual int siToITSLongitudinalAcceleration(float acceleration) = 0;
  virtual float itsToSILongitudinalAcceleration(int acceleration) = 0;

  virtual int siToITSCurvature(float curvature) = 0;
  virtual float itsToSICurvature(int curvature) = 0;

  virtual int siToITSYawRate(float yawRate) = 0;
  virtual float itsToSIYawRate(int yawRate) = 0;

  virtual int timestampToDeltaTime(unsigned long long int timestamp) = 0;
};

} // namespace level