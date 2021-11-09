#pragma once

#include <c2xcommon.h>

#ifdef __cplusplus
namespace c2x {
extern "C"
{
#endif

#define HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE 1
#define HIGH_FREQ_CONTAINER_TYPE_RSU 2

int SHARED_EXPORT SIMULINK_BLOCK CAMMessage(SIMULINK_NONTUN_PROP int stationID, 
    SIMULINK_NONTUN_PROP int heighFrequencyContainerType);
int SHARED_EXPORT createCAM(SIMULINK_NONTUN_PROP int stationID, 
    SIMULINK_NONTUN_PROP int heighFrequencyContainerType = HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE);
int SHARED_EXPORT deleteCAM(int stationID);

#pragma region Setter
int SHARED_EXPORT SIMULINK_BLOCK setCAMHeader(int stationID, int protocolVersion, int messageID);
int SHARED_EXPORT SIMULINK_BLOCK setCAMGenerationDeltaTime(int stationID, int generationDeltaTime);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicContainer(int stationID, int stationType, int latitude, int longitude, int confidenceMajor, 
    int confidenceMinor, int confidenceMajorOrientation, int altitudeValue, int altitudeConfidence);

int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequency(int stationID, int headingValue, int headingConfidence, 
    int speedValue, int speedConfidence, int driveDirection, int vehicleLengthValue, 
    int vehicleLengthConfidenceIndication, int vehicleWidth, int longitudinalAccelerationValue, 
    int longitudinalAccelerationConfidence, int curvatureValue, int curvatureConfidence, int curvatureCalculationMode, 
    int yawRateValue, int yawRateConfidence);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencyAccelerationControl(int stationID, SIMULINK_INPUT uint8_t *buffer, int bufferSize);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencyLanePosition(int stationID, int lanePosition);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(int stationID, int steeringWheelAngleValue, 
    int steeringWheelAngleConfidence);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencyLateralAcceleration(int stationID, int lateralAccelerationValue, 
    int lateralAccelerationConfidence);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(int stationID, int verticalAccelerationValue, 
    int verticalAccelerationConfidence);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencyPerformanceClass(int stationID, int performanceClass);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(int stationID, int protectedZoneLatitude, 
    int protectedZoneLongitude, int cenDsrcTollingZoneID);

int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerLowFrequency(int stationID, int vehicleRole, SIMULINK_INPUT uint8_t *exteriorLights, 
    int exteriorLightsSize);
int SHARED_EXPORT SIMULINK_BLOCK addCAMBasicVehicleContainerLowFrequencyPathPoint(int stationID, int deltaLatitude, 
    int deltaLongitude, int deltaAltitude, int deltaTime);
#pragma endregion

#pragma region Getter
int SHARED_EXPORT SIMULINK_BLOCK getCAMHeader(int stationID, int *protocolVersion, int *messageID);
int SHARED_EXPORT SIMULINK_BLOCK getCAMGenerationDeltaTime(int stationID, int *generationDeltaTime);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicContainer(int stationID, int *stationType, int *latitude, int *longitude, int *confidenceMajor, 
    int *confidenceMinor, int *confidenceMajorOrientation, int *altitudeValue, int *altitudeConfidence);

int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequency(int stationID, int *headingValue, int *headingConfidence, 
    int *speedValue, int *speedConfidence, int *driveDirection, int *vehicleLengthValue, 
    int *vehicleLengthConfidenceIndication, int *vehicleWidth, int *longitudinalAccelerationValue, 
    int *longitudinalAccelerationConfidence, int *curvatureValue, int *curvatureConfidence, 
    int *curvatureCalculationMode, int *yawRateValue, int *yawRateConfidence);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencyAccelerationControl(int stationID, SIMULINK_OUTPUT_MATRIX_A_1 uint8_t *buffer, 
    SIMULINK_NONTUN_PROP int bufferSize, int* actualBufferSize);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencyLanePosition(int stationID, int *lanePosition);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(int stationID, int *steeringWheelAngleValue, 
    int *steeringWheelAngleConfidence);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencyLateralAcceleration(int stationID, int *lateralAccelerationValue, 
    int *lateralAccelerationConfidence);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(int stationID, int *verticalAccelerationValue, 
    int *verticalAccelerationConfidence);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencyPerformanceClass(int stationID, int *performanceClass);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(int stationID, int *protectedZoneLatitude, 
    int *protectedZoneLongitude, int *cenDsrcTollingZoneID);

int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerLowFrequency(int stationID, int *vehicleRole, SIMULINK_OUTPUT_MATRIX_A_1 uint8_t *exteriorLights, 
    SIMULINK_NONTUN_PROP int exteriorLightsSize, int *actualExteriorLightsSize);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerLowFrequencyPathHistory(int stationID, SIMULINK_OUTPUT_MATRIX_A_4 int* pathHistory, 
    SIMULINK_NONTUN_PROP int pathHistorySize, int *actualPathHistorySize);
#pragma endregion

#pragma region De-/En-coding
int SHARED_EXPORT SIMULINK_BLOCK encodeCAM(int stationID, SIMULINK_OUTPUT_MATRIX_A_1 uint8_t* buffer, SIMULINK_NONTUN_PROP int bufferSize, int *actualBufferSize);
int SHARED_EXPORT SIMULINK_BLOCK decodeCAM(int* stationID, SIMULINK_OUTPUT_MATRIX_A_1 uint8_t* buffer, SIMULINK_NONTUN_PROP int bufferSize);
#pragma endregion

#pragma region NetworkService
int SHARED_EXPORT startCAMReceiver(int port);
int SHARED_EXPORT stopCAMReceiver();
int SHARED_EXPORT startCAMTransmitter(int port);
int SHARED_EXPORT stopCAMTransmitter();
int SHARED_EXPORT setCAMTransmissionFrequency(double f);
int SHARED_EXPORT setCAMIDsForTransmission(int *stationIDs_send, int size);
#pragma endregion

#ifdef __cplusplus
}
};
#endif