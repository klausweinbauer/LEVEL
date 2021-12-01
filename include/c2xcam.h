#pragma once

#include <c2xcommon.h>

#ifdef __cplusplus
namespace c2x {
extern "C"
{
#endif

#define HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE 1
#define HIGH_FREQ_CONTAINER_TYPE_RSU 2

int SHARED_EXPORT createCAM(int stationID, int heighFrequencyContainerType);
int SHARED_EXPORT deleteCAM(int stationID);

#pragma region Setter
int SHARED_EXPORT setCAMHeader(int stationID, int protocolVersion, int messageID);
int SHARED_EXPORT setCAMGenerationDeltaTime(int stationID, int generationDeltaTime);
int SHARED_EXPORT setCAMBasicContainer(int stationID, int stationType, int latitude, int longitude, int confidenceMajor, 
    int confidenceMinor, int confidenceMajorOrientation, int altitudeValue, int altitudeConfidence);

int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequency(int stationID, int headingValue, int headingConfidence, 
    int speedValue, int speedConfidence, int driveDirection, int vehicleLengthValue, 
    int vehicleLengthConfidenceIndication, int vehicleWidth, int longitudinalAccelerationValue, 
    int longitudinalAccelerationConfidence, int curvatureValue, int curvatureConfidence, int curvatureCalculationMode, 
    int yawRateValue, int yawRateConfidence);
int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencyAccelerationControl(int stationID, uint8_t *buffer, int bufferSize);
int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencyLanePosition(int stationID, int lanePosition);
int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(int stationID, int steeringWheelAngleValue, 
    int steeringWheelAngleConfidence);
int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencyLateralAcceleration(int stationID, int lateralAccelerationValue, 
    int lateralAccelerationConfidence);
int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(int stationID, int verticalAccelerationValue, 
    int verticalAccelerationConfidence);
int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencyPerformanceClass(int stationID, int performanceClass);
int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(int stationID, int protectedZoneLatitude, 
    int protectedZoneLongitude, int cenDsrcTollingZoneID);

int SHARED_EXPORT setCAMBasicVehicleContainerLowFrequency(int stationID, int vehicleRole, uint8_t *exteriorLights, 
    int exteriorLightsSize);
int SHARED_EXPORT addCAMBasicVehicleContainerLowFrequencyPathPoint(int stationID, int deltaLatitude, 
    int deltaLongitude, int deltaAltitude, int deltaTime);

int SHARED_EXPORT addCAMRSUContainerHighFrequencyProtectedCommunicationZone(int stationID, int protectedZoneType, 
    int expiryTime, int protectedZoneLatitude, int protectedZoneLongitude, int protectedZoneRadius, int protectedZoneID);
int SHARED_EXPORT clearCAMRSUContainerHighFrequencyProtectedCommunicationZones(int stationID);
#pragma endregion

#pragma region Getter
int SHARED_EXPORT getCAMHeader(int stationID, int *protocolVersion, int *messageID);
int SHARED_EXPORT getCAMGenerationDeltaTime(int stationID, int *generationDeltaTime);
int SHARED_EXPORT getCAMBasicContainer(int stationID, int *stationType, int *latitude, int *longitude, int *confidenceMajor, 
    int *confidenceMinor, int *confidenceMajorOrientation, int *altitudeValue, int *altitudeConfidence);

int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequency(int stationID, int *headingValue, int *headingConfidence, 
    int *speedValue, int *speedConfidence, int *driveDirection, int *vehicleLengthValue, 
    int *vehicleLengthConfidenceIndication, int *vehicleWidth, int *longitudinalAccelerationValue, 
    int *longitudinalAccelerationConfidence, int *curvatureValue, int *curvatureConfidence, 
    int *curvatureCalculationMode, int *yawRateValue, int *yawRateConfidence);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyAccelerationControl(int stationID, uint8_t *buffer, 
    int bufferSize, int* actualBufferSize);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyLanePosition(int stationID, int *lanePosition);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(int stationID, int *steeringWheelAngleValue, 
    int *steeringWheelAngleConfidence);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyLateralAcceleration(int stationID, int *lateralAccelerationValue, 
    int *lateralAccelerationConfidence);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(int stationID, int *verticalAccelerationValue, 
    int *verticalAccelerationConfidence);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyPerformanceClass(int stationID, int *performanceClass);
int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(int stationID, int *protectedZoneLatitude, 
    int *protectedZoneLongitude, int *cenDsrcTollingZoneID);

int SHARED_EXPORT getCAMBasicVehicleContainerLowFrequency(int stationID, int *vehicleRole, uint8_t *exteriorLights, 
    int exteriorLightsSize, int *actualExteriorLightsSize);
int SHARED_EXPORT getCAMBasicVehicleContainerLowFrequencyPathHistory(int stationID, int* pathHistory, 
    int pathHistorySize, int *actualPathHistorySize);

int SHARED_EXPORT getCAMRSUContainerHighFrequencyProtectedCommunicationZone(int stationID, int index, int* buffer, int bufferSize);
#pragma endregion

#pragma region De-/En-coding
int SHARED_EXPORT encodeCAM(int stationID, uint8_t* buffer, int bufferSize, int *actualBufferSize);
int SHARED_EXPORT decodeCAM(int* stationID, uint8_t* buffer, int bufferSize);
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