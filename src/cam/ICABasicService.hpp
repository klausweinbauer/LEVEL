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

#include <CAMWrapper.hpp>
#include <level.h>

namespace level::cam {

class ICABasicService {

public:
  virtual ~ICABasicService() {}

  /**
   * @brief Configure the cooperative awareness basic service.
   *
   * @param config Settings to apply.
   */
  virtual void configure(CABasicServiceConfig config) = 0;

  /**
   * @brief Get the current cooperative awareness basic service configuration.
   *
   * @return CABasicServiceConfig Current configuration.
   */
  virtual CABasicServiceConfig getConfiguration() = 0;

  /**
   * @brief Get the current CAM generation frequency.
   *
   * @return float Frequency in Hz.
   */
  virtual float getCAMGenerationFrequency() = 0;

  /**
   * @brief Get the current local CAM state.
   *
   * @return CAMWrapper Local CAM instance.
   */
  virtual CAMWrapper cam() = 0;

  /**
   * @brief Try to get the most recent data for a specific station.
   *
   * @param stationID StationID from where to get the data.
   * @param cam Pointer to message where to store the data.
   * @return true A message exists for the provided station id and was written
   * to cam.
   * @return false No message exists for station id. Cam remains unchanged.
   */
  virtual bool tryGetCAM(unsigned int stationID, CAMWrapper *cam) = 0;

  virtual void setHeading(float heading) = 0;

  virtual void setSpeed(float speed) = 0;

  virtual void setDriveDirection(DriveDirectionType direction) = 0;

  virtual void setAcceleration(float acceleration) = 0;

  virtual void setCurvature(float radius) = 0;

  virtual void setYawRate(float yawRate) = 0;
};

} // namespace level::cam