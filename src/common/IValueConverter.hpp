#pragma once

#include <TimestampIts.h>

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

  virtual int siToITSLongitude(float longitude) = 0;
  virtual float itsToSILongitude(int longitude) = 0;

  virtual int siToITSLatitude(float latitude) = 0;
  virtual float itsToSILatitude(int latitude) = 0;

  /**
   * @brief Compute the distance measure in meters between two points given
   * their longitudes and latitudes.
   *
   * @param long1 Longitude of point A.
   * @param lat1 Latitude of point A.
   * @param long2 Longitude of point B.
   * @param lat2 Latitude of point B.
   * @return float Distance between A and B in meters.
   */
  virtual float distance(float long1, float lat1, float long2, float lat2) = 0;

  /**
   * @brief Convert a timestamp in milliseconds since 2004-01-01
   * 00:00:00:000 UTC into a ITS Timestamp object. You should always assign this
   * timestamp object to a CAM or DENM message because this method allocates
   * memory that otherwise would be lost if not explicitly freed.
   *
   * @param ms Milliseconds since 2004-01-01 00:00:00:000 UTC.
   *
   * @return A timestamp object.
   */
  virtual TimestampIts_t *siToITSTimestamp(unsigned long long int ms) = 0;

  /**
   * @brief Convert a timestamp to milliseconds since 2004-01-01 00:00:00:000
   * UTC.
   *
   * @param timestamp A timestamp reference.
   *
   * @return Milliseconds since 2004-01-01 00:00:00:000 UTC.
   */
  virtual unsigned long long int
  itsToSITimestamp(const TimestampIts_t *timestamp) = 0;
};

} // namespace level