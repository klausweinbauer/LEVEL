#pragma once

#include <CAMWrapper.hpp>
#include <ValueConverter.hpp>
#include <functional>
#include <level.h>

namespace level::cam {

class CABasicService {
private:
  std::shared_ptr<IValueConverter> _valueConverter;
  CAMWrapper _cam;
  CABasicServiceConfig _config;

public:
  virtual ~CABasicService() {}

  CABasicService(std::shared_ptr<IValueConverter> valueConverter);

  std::function<void(ITSPDUHeader header)> receiveCallback;

  /**
   * @brief Configure the cooperative awareness basic service.
   *
   * @param config Settings to apply.
   */
  virtual void configure(CABasicServiceConfig config);

  /**
   * @brief Get the current cooperative awareness basic service configuration.
   *
   * @return CABasicServiceConfig Current configuration.
   */
  virtual CABasicServiceConfig getConfiguration();

  /**
   * @brief Get the current CAM generation frequency.
   *
   * @return float Frequency in Hz.
   */
  virtual float getCAMGenerationFrequency();

  /**
   * @brief Get a reference to the current local CAM.
   *
   * @return CAMWrapper* Local CAM instance.
   */
  virtual CAMWrapper &cam();

  /**
   * @brief Get the most recent data for a specific station.
   *
   * @param stationID StationID from where to get the data.
   * @return CAMWrapper Message.
   */
  virtual CAMWrapper getCAM(unsigned int stationID);

  virtual void setHeading(float heading);

  virtual void setSpeed(float speed);

  virtual void setDriveDirection(DriveDirectionType direction);

  virtual void setAcceleration(float acceleration);

  virtual void setCurvature(float radius);

  virtual void setYawRate(float yawRate);
};

} // namespace level::cam