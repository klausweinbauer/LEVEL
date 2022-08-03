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

public:
  virtual ~CABasicService() {}

  CABasicService(std::shared_ptr<IValueConverter> valueConverter);

  std::function<void(ITSPDUHeader header)> receiveCallback;

  /**
   * @brief Configure the cooperative awareness basic service.
   *
   * @param config Settings to apply.
   */
  void configure(CABasicServiceConfig config);

  /**
   * @brief Get the current cooperative awareness basic service configuration.
   *
   * @return CABasicServiceConfig Current configuration.
   */
  CABasicServiceConfig getConfiguration();

  /**
   * @brief Get the current CAM generation frequency.
   *
   * @return float Frequency in Hz.
   */
  float getCAMGenerationFrequency();

  /**
   * @brief Get a reference to the current local CAM.
   *
   * @return CAMWrapper* Local CAM instance.
   */
  CAMWrapper &cam();

  /**
   * @brief Get the most recent data for a specific station.
   *
   * @param stationID StationID from where to get the data.
   * @return CAMWrapper Message.
   */
  CAMWrapper getCAM(unsigned int stationID);

  void setHeading(float heading);

  void setSpeed(float speed);

  void setDriveDirection(DriveDirectionType direction);

  void setAcceleration(float acceleration);

  void setCurvature(float radius);

  void setYawRate(float yawRate);
};

} // namespace level::cam