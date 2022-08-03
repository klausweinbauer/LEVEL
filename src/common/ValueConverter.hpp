#pragma once

#include <IValueConverter.hpp>

namespace level {

class ValueConverter : public IValueConverter {
public:
  virtual ~ValueConverter() {}

  virtual int siToITSHeading(float heading) override;
  virtual float itsToSIHeading(int heading) override;

  virtual int siToITSSpeed(float speed) override;
  virtual float itsToSISpeed(int speed) override;

  virtual int siToITSLongitudinalAcceleration(float acceleration) override;
  virtual float itsToSILongitudinalAcceleration(int acceleration) override;

  virtual int siToITSCurvature(float curvature) override;
  virtual float itsToSICurvature(int curvature) override;

  virtual int siToITSYawRate(float yawRate) override;
  virtual float itsToSIYawRate(int yawRate) override;
};

} // namespace level