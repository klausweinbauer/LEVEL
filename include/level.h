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
 * @brief Function to set a detailed error message.
 *
 * @param buffer The buffer where the error message is stored.
 * @param size The length of the error message.
 */
void SHARED_EXPORT setLastErrMsg(const char *buffer, int size);

/**
 * @brief Clear the last error message.
 *
 */
void SHARED_EXPORT clearLastErrMsg();

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
} EncodingType;

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

#pragma endregion // Enums

#pragma region Structs

/**
 * @brief Vehicle data provider.
 *
 */
// TODO error: field ‘setHeading’ declared as a function
typedef struct VDP {
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

} VDP_t;

typedef struct CABasicServiceConfig {
  unsigned int stationID;
  StationType stationType;
} CABasicServiceConfig_t;

typedef struct ITSPDUHeader {
  unsigned int stationID;
  long protocolVersion;
  long messageID;
} ITSPDUHeader_t;

typedef struct CAMBasicContainerData {
  StationType stationType;
  float latitude;
  float longitude;
  float altitude;
} CAMBasicContainerData_t;

typedef struct CAMBasicVehicleContainerHighFrequencyData {
  float headingValue;
  float speedValue;
  DriveDirectionType driveDirection;
  float vehicleLength;
  float vehicleWidth;
  float longitudinalAccelerationValue;
  float curvatureValue;
  float yawRateValue;
} CAMBasicVehicleContainerHighFrequencyData_t;

#pragma endregion // Structs

#pragma region Utility methods

/**
 * @brief Returns the library version.
 *
 * @return const char* Version string.
 */
SHARED_EXPORT const char *getVersion();

/**
 * @brief Get a pointer to the vehicle data provider. Use the VDP to update the
 * current vehicle state.
 *
 * @return VDP_t* Pointer to the vehicle data provider.
 */
SHARED_EXPORT VDP_t *getVDP();

#pragma endregion // Configuration methods

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
                                       CAMBasicContainerData_t *container);

/**
 * @brief Returns the most recent high frequency data for a vehicle.
 *
 * @param stationID StationID of the vehicle.
 * @param container Container where the data is stored.
 * @return int Returns 0 on success or an error code.
 */
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequency(
    int stationID, CAMBasicVehicleContainerHighFrequencyData_t *container);

// TODO getter methods for LFC, SVC, RSUHFC

#pragma endregion // CAM methods

#pragma region DENM methods

#pragma endregion // DENM methods

#ifdef __cplusplus
}
};
#endif