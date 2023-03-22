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
};

} // namespace level