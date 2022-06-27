/**
 * @file c2xcam_config.h
 * @author Klaus Weinbauer
 * @brief Provides a configuration and management API for the CAM-Service.
 * @version 0.1
 * @date 2022-06-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <c2xcommon.h>

#ifdef __cplusplus
namespace c2x {
extern "C" {
#endif

/**
 * @brief Enumeration for possible HighFrequencyContainer types.
 *
 */
enum HighFrequencyContainerType {
  NONE = 0,
  /**< No HighFrequencyContainer specified */
  BasicVehicleContainerHighFrequencyType = 1,
  /**< BasicVehicleContainer as HighFrequencyContainer */
  RsuContainerHighFrequencyType = 2
  /**< RsuContainer as HighFrequencyContainer */
};

/**
 * @brief Create a new CAM for a Station.
 *
 * @param stationID The Station ID of the new message.
 * @return Returns the new ID or an \ref c2xerror.h "error code".
 */
int SHARED_EXPORT createCAM(int stationID);

/**
 * @brief Delete an existing CAM by its Station ID.
 *
 * @param stationID The Station ID of the message that should be deleted.
 * @return int Returns 0 on success or an \ref c2xerror.h "error code".
 */
int SHARED_EXPORT deleteCAM(int stationID);

/**
 * @brief Set the type of the HighFrequencyContainer for a specific station.
 *
 * @param stationID The Station ID for which the container should be configured.
 * @param type The \ref HighFrequencyContainerType "type" of the high frequency
 * container.
 * @return int Returns 0 on success or an \ref c2xerror.h "error code".
 */
int SHARED_EXPORT defineCAMHighFrequencyContainer(int stationID, int type);

#ifdef __cplusplus
}
};
#endif