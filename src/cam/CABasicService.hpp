#pragma once

#include <CAMWrapper.hpp>
#include <functional>
#include <level.h>

namespace level::cam {

class CABasicService {

public:
  virtual ~CABasicService();

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
   * @brief Get the most recent data for a specific station.
   *
   * @param stationID StationID from where to get the data.
   * @return CAMWrapper Message.
   */
  CAMWrapper getCAM(unsigned int stationID);
};

} // namespace level::cam