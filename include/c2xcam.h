#pragma once

#include <c2xcommon.h>

#ifdef __cplusplus
namespace c2x {
extern "C"
{
#endif

#define SIMULINK_BLOCK
#define SIMULINK_NONTUNABLE_PROPERTY

#define HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE 1
#define HIGH_FREQ_CONTAINER_TYPE_RSU 2

int SHARED_EXPORT createCAM(int heighFrequencyContainerType = HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE);
int SHARED_EXPORT deleteCAM(int id);

#pragma region Setter
int SHARED_EXPORT SIMULINK_BLOCK setCAMHeader(int id, int protocolVersion, int messageID, int stationID);
int SHARED_EXPORT SIMULINK_BLOCK setCAMGenerationDeltaTime(int id, int generationDeltaTime);
int SHARED_EXPORT setCAMBasicContainer(int id, int stationType, Position position);

int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequency(int id, int headingValue, int headingConfidence, 
    int speedValue, int speedConfidence, int driveDirection, int vehicleLengthValue, 
    int vehicleLengthConfidenceIndication, int vehicleWidth, int longitudinalAccelerationValue, 
    int longitudinalAccelerationConfidence, int curvatureValue, int curvatureConfidence, int curvatureCalculationMode, 
    int yawRateValue, int yawRateConfidence);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencyAccelerationControl(int id, uint8_t *buffer, int size);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencyLanePosition(int id, int lanePosition);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(int id, int steeringWheelAngleValue, 
    int steeringWheelAngleConfidence);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencyLateralAcceleration(int id, int lateralAccelerationValue, 
    int lateralAccelerationConfidence);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(int id, int verticalAccelerationValue, 
    int verticalAccelerationConfidence);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencyPerformaceClass(int id, int performanceClass);
int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(int id, int protectedZoneLatitude, 
    int protectedZoneLongitude, int cenDsrcTollingZoneID);

int SHARED_EXPORT SIMULINK_BLOCK setCAMBasicVehicleContainerLowFrequency(int id, int vehicleRole, uint8_t *exteriorLights, 
    int exteriorLightsSize);
int SHARED_EXPORT SIMULINK_BLOCK addCAMBasicVehicleContainerLowFrequencyPathPoint(int id, int deltaLatitude, 
    int deltaLongitude, int deltaAltitude, int deltaTime);
#pragma endregion

#pragma region Getter
int SHARED_EXPORT getCAMHeader(int id, int *protocolVersion, int *messageID, int *stationID);
int SHARED_EXPORT SIMULINK_BLOCK getCAMHeader_recv(int stationId, int *protocolVersion, int *messageID, int *stationID);
int SHARED_EXPORT getCAMGenerationDeltaTime(int id, int *generationDeltaTime);
int SHARED_EXPORT SIMULINK_BLOCK getCAMGenerationDeltaTime_recv(int stationId, int *generationDeltaTime);
int SHARED_EXPORT getCAMBasicContainer(int id, int *stationType, int *latitude, int *longitude, int *confidenceMajor, 
    int *confidenceMinor, int *confidenceMajorOrientation, int *altitudeValue, int *altitudeConfidence);
int SHARED_EXPORT getCAMBasicContainer_recv(int stationId, int *stationType, int *latitude, int *longitude, 
    int *confidenceMajor, int *confidenceMinor, int *confidenceMajorOrientation, int *altitudeValue, 
    int *altitudeConfidence);

int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequency(int id, int *headingValue, int *headingConfidence, 
    int *speedValue, int *speedConfidence, int *driveDirection, int *vehicleLengthValue, 
    int *vehicleLengthConfidenceIndication, int *vehicleWidth, int *longitudinalAccelerationValue, 
    int *longitudinalAccelerationConfidence, int *curvatureValue, int *curvatureConfidence, 
    int *curvatureCalculationMode, int *yawRateValue, int *yawRateConfidence);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequency_recv(int stationId, int *headingValue, 
    int *headingConfidence, int *speedValue, int *speedConfidence, int *driveDirection, int *vehicleLengthValue, 
    int *vehicleLengthConfidenceIndication, int *vehicleWidth, int *longitudinalAccelerationValue, 
    int *longitudinalAccelerationConfidence, int *curvatureValue, int *curvatureConfidence, 
    int *curvatureCalculationMode, int *yawRateValue, int *yawRateConfidence);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyAccelerationControl(int id, uint8_t *buffer, 
    int size, int* actualSize);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencyAccelerationControl_recv(int id, 
    uint8_t *buffer, SIMULINK_NONTUNABLE_PROPERTY int size, int* actualSize);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyLanePosition(int id, int *lanePosition);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencyLanePosition_recv(int id, int *lanePosition);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(int id, int *steeringWheelAngleValue, 
    int *steeringWheelAngleConfidence);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencySteeringWheelAngle_recv(int id, int *steeringWheelAngleValue, 
    int *steeringWheelAngleConfidence);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyLateralAcceleration(int id, int *lateralAccelerationValue, 
    int *lateralAccelerationConfidence);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencyLateralAcceleration_recv(int id, 
    int *lateralAccelerationValue, int *lateralAccelerationConfidence);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(int id, int *verticalAccelerationValue, 
    int *verticalAccelerationConfidence);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencyVerticalAcceleration_recv(int id, 
    int *verticalAccelerationValue, int *verticalAccelerationConfidence);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyPerformaceClass(int id, int *performanceClass);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencyPerformaceClass_recv(int id, int *performanceClass);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(int id, int *protectedZoneLatitude, 
    int *protectedZoneLongitude, int *cenDsrcTollingZoneID);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone_recv(int id, int *protectedZoneLatitude, 
    int *protectedZoneLongitude, int *cenDsrcTollingZoneID);

int SHARED_EXPORT getCAMBasicVehicleContainerLowFrequency(int id, int *vehicleRole, uint8_t *exteriorLights, 
    int exteriorLightsSize, int *actualExteriorLightsSize);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerLowFrequency_recv(int stationId, int *vehicleRole, 
    uint8_t *exteriorLights, SIMULINK_NONTUNABLE_PROPERTY int exteriorLightsSize, int *actualExteriorLightsSize);
int SHARED_EXPORT getCAMBasicVehicleContainerLowFrequencyPathHistory(int id, int* pathHistory, 
    int pathHistorySize, int *actualPathHistorySize);
int SHARED_EXPORT SIMULINK_BLOCK getCAMBasicVehicleContainerLowFrequencyPathHistory_recv(int stationId, 
    int* pathHistory, SIMULINK_NONTUNABLE_PROPERTY int pathHistorySize, int *actualPathHistorySize);
#pragma endregion

#pragma region De-/En-coding
int SHARED_EXPORT encodeCAM(int id, uint8_t* buffer, int size);
int SHARED_EXPORT encodeCAM_recv(int stationId, uint8_t* buffer, int size);
int SHARED_EXPORT decodeCAM(int* id, uint8_t* buffer, int size);
int SHARED_EXPORT decodeCAMOverride(int id, uint8_t* buffer, int size);
#pragma endregion

#pragma region NetworkService
int SHARED_EXPORT startCAMReceiver(int port);
int SHARED_EXPORT stopCAMReceiver();
int SHARED_EXPORT startCAMTransmitter(int port);
int SHARED_EXPORT stopCAMTransmitter();
#pragma endregion

#ifdef __cplusplus
}
};
#endif