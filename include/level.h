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

#ifdef _WIN32
#ifndef SHARED_EXPORT
#define SHARED_EXPORT __declspec(dllexport)
#endif
#else
#define SHARED_EXPORT
#endif

#ifdef __cplusplus
namespace level {

extern "C" {
#endif

#pragma region Error codes and related methods
/**
 * @brief General error code
 *
 */
#define ERR -1

/**
 * @brief Memory allocation for new messages failed
 *
 */
#define ERR_ALLOC_FAILED -2

/**
 * @brief Null reference exception
 *
 */
#define ERR_NULL -3

/**
 * @brief The provided buffer is to small
 *
 */
#define ERR_BUFFER_OVERFLOW -4

/**
 * @brief Message encoding failed
 *
 */
#define ERR_ENCODE -5

/**
 * @brief Message decoding failed
 *
 */
#define ERR_DECODE -6

/**
 * @brief Starting the receiver failed
 *
 */
#define ERR_RECEIVER_START -7

/**
 * @brief Starting the transmitter failed
 *
 */
#define ERR_TRANSMITTER_START -8

/**
 * @brief Argument which shouldn't be null is null
 *
 */
#define ERR_ARG_NULL -9

/**
 * @brief Index is out of range
 *
 */
#define ERR_INDEX_OUT_OF_RANGE -10

/**
 * @brief Provided argument is invalid
 *
 */
#define ERR_INVALID_ARG -11

/**
 * @brief Message does not exist in database
 *
 */
#define ERR_MSG_NOT_FOUND -12

/**
 * @brief Service detected an invalid operation
 *
 */
#define ERR_INVALID_OPERATION -13

/**
 * @brief Invalid type for high frequency container
 *
 */
#define ERR_HIGH_FREQ_CONTAINER_TYPE -40

/**
 * @brief Invalid type for low frequency container
 *
 */
#define ERR_LOW_FREQ_CONTAINER_TYPE -41

/**
 * @brief CAM with ID already exists
 *
 */
#define ERR_CAM_ALREADY_EXISTS -42

/**
 * @brief Invalid type for special vehicle container
 *
 */
#define ERR_SPECIAL_VEHICLE_CONTAINER_TYPE -43

/**
 * @brief DENM with ID and Sequence Number already exists
 *
 */
#define ERR_DENM_ALREADY_EXISTS -60

/**
 * @brief This function provides a general description for an error code.
 *
 * @param err The \ref level_error.h "error code" for which the description
 * should be read.
 * @param buffer This is the buffer where the error message is to be copied.
 * @param size The size of the buffer.
 */
void SHARED_EXPORT getErrMsg(int err, char *buffer, int size);

/**
 * @brief Provides a description for the last error.
 *
 * @param buffer This is the buffer where the error message is to be copied.
 * @param size The size of the buffer.
 * @param actualSize Returns the actual length of the error message.
 */
void SHARED_EXPORT getLastErrMsg(char *buffer, int size, int *actualSize);

/**
 * @brief Clear the last error message.
 *
 */
void SHARED_EXPORT clearErrMsg();

#pragma endregion

#pragma region Enums

/**
 * @brief Collection of supported encoding standards for message
 * en- and decoding
 *
 */
typedef enum {
  XER_BASIC,
  /**<
    XML Encoding Rules according to ITU-T.
    Use this encoding to achieve better human readability.
  */
  XER_CANONICAL,
  /**<
    XML Encoding Rules according to ITU-T.
    Use this encoding to achieve better human readability.
  */
  DER_BER
  /**<
    Basic and Distiributed Encoding Rules according to ITU-T.
    Use this for a compact encoding and smaller sizes for packages sent over
    the network.
  */
} MsgEncodingType;

typedef enum {
  StationType_Unknown = 0,
  StationType_Pedestrian = 1,
  StationType_Cyclist = 2,
  StationType_Moped = 3,
  StationType_Motorcycle = 4,
  StationType_PassengerCar = 5,
  StationType_Bus = 6,
  StationType_LightTruck = 7,
  StationType_HeavyTruck = 8,
  StationType_Trailer = 9,
  StationType_SpecialVehicles = 10,
  StationType_Tram = 11,
  StationType_RoadSideUnit = 15
} StationType;

typedef enum {
  DriveDirection_Forward = 0,
  DirveDirection_Backward = 1,
  DriveDirection_Unavailable = 2
} DriveDirectionType;

typedef enum {
  EventType_Reserved = 0,
  EventType_TrafficCondition = 1,
  EventType_Accident = 2,
  EventType_Roadworks = 3,
  EventType_Impassability = 5,
  EventType_AdverseWeatherCondition_Adhesion = 6,
  EventType_Aquaplannning = 7,
  EventType_HazardousLocation_SurfaceCondition = 9,
  EventType_HazardousLocation_ObstacleOnTheRoad = 10,
  EventType_HazardousLocation_AnimalOnTheRoad = 11,
  EventType_HumanPresenceOnTheRoad = 12,
  EventType_WrongWayDriving = 14,
  EventType_RescueAndRecoveryWorkInProgress = 15,
  EventType_AdverseWeatherCondition_ExtremeWeatherCondition = 17,
  EventType_AdverseWeatherCondition_Visibility = 18,
  EventType_AdverseWeatherCondition_Precipitation = 19,
  EventType_SlowVehicle = 26,
  EventType_DangerousEndOfQueue = 27,
  EventType_VehicleBreakdown = 91,
  EventType_PostCrash = 92,
  EventType_HumanProblem = 93,
  EventType_StationaryVehicle = 94,
  EventType_EmergencyVehicleApproaching = 95,
  EventType_HazardousLocation_DangerousCurve = 96,
  EventType_CollisionRisk = 97,
  EventType_SignalViolation = 98,
  EventType_DangerousSituation = 99
} EventType;

typedef enum {
  DENMState_Active,
  DENMState_Cancelled,
  DENMState_Negated
} DENMState;

#pragma endregion // Enums

#pragma region Structs

typedef struct CABasicServiceConfig {
  unsigned int stationID;
  StationType stationType;
  // TODO MsgEncodingType encoding;
} CABasicServiceConfig_t;

typedef struct DENBasicServiceConfig {
  unsigned int stationID;
  StationType stationType;
  // TODO MsgEncodingType encoding;
} DENBasicServiceConfig_t;

typedef struct ActionId {
  unsigned int stationID;
  int sequenceNumber;
} ActionId_t;

typedef struct ITSPDUHeader {
  unsigned int stationID;
  long protocolVersion;
  long messageID;
} ITSPDUHeader_t;

typedef struct Position {
  float latitude;
  float longitude;
  float altitude;
} Position_t;

typedef struct CAMBasicContainer {
  StationType stationType;
  // TODO change to position type
  float latitude;
  float longitude;
  float altitude;
} CAMBasicContainer_t;

typedef struct CAMBasicVehicleContainerHF {
  float headingValue;
  float speedValue;
  DriveDirectionType driveDirection;
  float vehicleLength;
  float vehicleWidth;
  float longitudinalAccelerationValue;
  float curvatureValue;
  float yawRateValue;
} CAMBasicVehicleContainerHF_t;

typedef struct DENMManagementConatiner {
  unsigned long long int detectionTime;

} DENMManagementContainer_t;

#pragma endregion // Structs

#pragma region Utility methods

/**
 * @brief Returns the library version.
 *
 * @return const char* Version string.
 */
SHARED_EXPORT const char *getVersion();

/**
 * @brief Set the current heading value in degree with regards to the true
 * north.
 *
 * @param heading Heading in degree.
 */
void setHeading(float heading);

/**
 * @brief Set the current speed value.
 *
 * @param speed Speed in m/s;
 */
void setSpeed(float speed);

/**
 * @brief Set the current drive direction.
 *
 * @param direction Direction.
 */
void setDriveDirection(DriveDirectionType direction);

/**
 * @brief Set the current longitudinal acceleration value.
 *
 * @param acceleration Acceleration in m/s².
 */
void setAcceleration(float acceleration);

/**
 * @brief Set the current curvature value. Positive values indicate a turning
 * curve to the left hand side of the driver. The value shall be set to 0 when
 * the vehicle is moving straight.
 *
 * @param curvature Curvature radius in m. 0 for moving stright. Negative for
 * left hand turn.
 */
void setCurvature(float radius);

/**
 * @brief Set the current yaw rate. Yaw rate denotes the vehicle rotation
 * around the centre of mass of the empty vehicle. The leading sign denotes
 * the direction of rotation. The value is negative if the motion is clockwise
 * when viewing from the top.
 *
 * @param yawRate Value in deg/sec.
 */
void setYawRate(float yawRate);

#pragma endregion

#pragma region CAM methods

/**
 * @brief Configure the cooperative awareness basic service.
 *
 * @param config Configuration.
 * @return int Returns 0 on success or an error code.
 */
int SHARED_EXPORT configureCABasicService(CABasicServiceConfig_t config);

/**
 * @brief Returns the current cooperative awareness basic service configuration.
 *
 * @return CABasicServiceConfig Current configuration.
 */
CABasicServiceConfig_t SHARED_EXPORT getCABasicServiceConfig();

/**
 * @brief Returns the most recent data for a specific station.
 *
 * @param stationID StationID from where to get the data.
 * @param container Container where the data is stored.
 * @return int Returns 0 on success or an error code.
 */
int SHARED_EXPORT getCAMBasicContainer(int stationID,
                                       CAMBasicContainer_t *container);

/**
 * @brief Returns the most recent high frequency data for a vehicle.
 *
 * @param stationID StationID of the vehicle.
 * @param container Container where the data is stored.
 * @return int Returns 0 on success or an error code.
 */
int SHARED_EXPORT getCAMBasicVehicleContainerHF(
    int stationID, CAMBasicVehicleContainerHF_t *container);

// TODO getter methods for LFC, SVC, RSUHFC

#pragma endregion // CAM methods

#pragma region DENM methods

/**
 * @brief Configure the decentralized environmental notification basic service.
 *
 * @param config Configuration.
 * @return int Returns 0 on success or an error code.
 */
int SHARED_EXPORT configureDENBasicService(DENBasicServiceConfig_t config);

/**
 * @brief Returns the current decentralized environmental notification basic
 * service configuration.
 *
 * @return DENBasicServiceConfig Current configuration.
 */
DENBasicServiceConfig_t SHARED_EXPORT getDENBasicServiceConfig();

#pragma endregion // DENM methods

#ifdef __cplusplus
}
};
#endif