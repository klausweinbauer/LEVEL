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
};

} // namespace level