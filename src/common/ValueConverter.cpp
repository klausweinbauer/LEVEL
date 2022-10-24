#include <Exception.hpp>
#include <ValueConverter.hpp>

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>

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

int ValueConverter::siToITSLongitude(float longitude) {
  if (abs(longitude) > 180) {
    throw Exception(ERR_INVALID_ARG,
                    "Argument 'longitude' should be between -180 and 180.");
  }
  return ROUND(longitude * 10000000);
}

float ValueConverter::itsToSILongitude(int longitude) {
  if (abs(longitude) > 1800000000) {
    throw Exception(
        ERR_INVALID_ARG,
        "Argument 'longitude' should be between -1800000000 and 1800000000");
  }
  return longitude / 10000000.0;
}

int ValueConverter::siToITSLatitude(float latitude) {
  if (abs(latitude) > 90) {
    throw Exception(ERR_INVALID_ARG,
                    "Argument 'latitude' should be between -90 and 90.");
  }
  return ROUND(latitude * 10000000);
}

float ValueConverter::itsToSILatitude(int latitude) {
  if (abs(latitude) > 900000000) {
    throw Exception(
        ERR_INVALID_ARG,
        "Argument 'latitude' should be between -900000000 and 900000000");
  }
  return latitude / 10000000.0;
}

// Haversine formula
float ValueConverter::distance(float long1, float lat1, float long2,
                               float lat2) {
  double R =
      6371; // Radius of earth in km (Volumetric mean)
            // https://nssdc.gsfc.nasa.gov/planetary/factsheet/earthfact.html
  double dLat = lat2 * M_PI / 180 - lat1 * M_PI / 180;
  double dLon = long2 * M_PI / 180 - long1 * M_PI / 180;
  double a = sin(dLat / 2) * sin(dLat / 2) + cos(lat1 * M_PI / 180) *
                                                 cos(lat2 * M_PI / 180) *
                                                 sin(dLon / 2) * sin(dLon / 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double d = R * c;
  return d * 1000;
}

void ValueConverter::siToITSTimestamp(unsigned long long int ms,
                                      TimestampIts_t &timestamp) {
  if (timestamp.buf != nullptr)
    free(timestamp.buf);

  timestamp.size = 8;
  timestamp.buf = (uint8_t *)malloc(sizeof(uint8_t) * timestamp.size);
  for (int i = 0; i < timestamp.size; i++) {
    timestamp.buf[i] = (ms >> ((timestamp.size - i - 1) * 8)) & 0xFF;
  }
}
unsigned long long int
ValueConverter::itsToSITimestamp(const TimestampIts_t &timestamp) {
  if (timestamp.buf == nullptr) {
    throw Exception(ERR_NULL, "Timestamp buffer is null.");
  }

  // Not sure how the timestamp is actually encoded
  if (timestamp.size > 8) {
    std::stringstream ss;
    ss << "The timestamp buffer size is to big. Expected max. size 8 "
          "bytes, actual size "
       << timestamp.size << " bytes.";
    throw Exception(ERR, ss.str());
  }

  unsigned long long int ms = 0;
  for (int i = 0; i < timestamp.size; i++) {
    ms <<= 8;
    ms += timestamp.buf[i];
  }

  return ms;
}

} // namespace level