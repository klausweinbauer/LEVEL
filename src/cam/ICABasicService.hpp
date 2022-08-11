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

  virtual void setHeading(float heading) = 0;

  virtual void setSpeed(float speed) = 0;

  virtual void setDriveDirection(DriveDirectionType direction) = 0;

  virtual void setAcceleration(float acceleration) = 0;

  virtual void setCurvature(float radius) = 0;

  virtual void setYawRate(float yawRate) = 0;
};

} // namespace level::cam