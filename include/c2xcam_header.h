/**
 * @file c2xcam_header.h
 * @author Klaus Weinbauer
 * @brief API to set and read properties of CAMs.
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

#pragma region Setter

/**
 * @brief Set CAM header properties.
 *
 * @param stationID The Station ID of the message where to set the properties.
 * @param protocolVersion
 * @param messageID
 * @return int Returns 0 on success or an \ref c2xerror.h "error code".
 */
int SHARED_EXPORT setCAMHeader(int stationID, int protocolVersion,
                               int messageID);

/**
 * @brief Set CAM timestamp.
 *
 * @param stationID The Station ID of the message where to set the timestamp.
 * @param generationDeltaTime
 * @return int Returns 0 on success or an \ref c2xerror.h "error code".
 */
int SHARED_EXPORT setCAMGenerationDeltaTime(int stationID,
                                            int generationDeltaTime);

#pragma endregion

#pragma region Getter

/**
 * @brief Get CAM header properties.
 *
 * @param stationID The Station ID of the message from where to read the
 * properties.
 * @param protocolVersion
 * @param messageID
 * @return int Returns 0 on success or an \ref c2xerror.h "error code".
 */
int SHARED_EXPORT getCAMHeader(int stationID, int *protocolVersion,
                               int *messageID);

/**
 * @brief Get CAM timestamp.
 *
 * @param stationID The Station ID of the message where to set the timestamp.
 * @param generationDeltaTime
 * @return int Returns 0 on success or an \ref c2xerror.h "error code".
 */
int SHARED_EXPORT getCAMGenerationDeltaTime(int stationID,
                                            int *generationDeltaTime);

#pragma endregion

#ifdef __cplusplus
}
};
#endif