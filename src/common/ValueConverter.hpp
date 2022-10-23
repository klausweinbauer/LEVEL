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

  virtual int timestampToDeltaTime(unsigned long long int timestamp) override;

  virtual int siToITSLongitude(float longitude) override;
  virtual float itsToSILongitude(int longitude) override;

  virtual int siToITSLatitude(float latitude) override;
  virtual float itsToSILatitude(int latitude) override;

  virtual float distance(float long1, float lat1, float long2,
                         float lat2) override;

  virtual TimestampIts_t *siToITSTimestamp(unsigned long long int ms) override;
  virtual unsigned long long int
  itsToSITimestamp(const TimestampIts_t *timestamp) override;
};

} // namespace level