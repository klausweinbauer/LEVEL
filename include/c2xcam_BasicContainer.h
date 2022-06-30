/**
 * @file c2xcam_BasicContainer.h
 * @author Klaus Weinbauer
 * @brief API to set and read properties of CAMs.
 * @version 0.1
 * @date 2022-06-27
 *
 * @copyright Copyright (c) 2022
 *
 * @details The stationID is always the id of the message where the values are
 * set or read from. The setter and getter methods return 0 on success or an
 * \ref c2xerror.h "error code" if something went wrong. Before using some of
 * the (set/get)BasicVehicleContainerHighFrequency methods, make sure to
 * configure the HighFrequencyContainer correctly with
 * defineCAMHighFrequencyContainer().
 */

#pragma once

#include <c2xcommon.h>

#ifdef __cplusplus
namespace c2x {
extern "C" {
#endif

int SHARED_EXPORT setCAMBasicContainer(int stationID, int stationType,
                                       int latitude, int longitude,
                                       int confidenceMajor, int confidenceMinor,
                                       int confidenceMajorOrientation,
                                       int altitudeValue,
                                       int altitudeConfidence);

int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequency(
    int stationID, int headingValue, int headingConfidence, int speedValue,
    int speedConfidence, int driveDirection, int vehicleLengthValue,
    int vehicleLengthConfidenceIndication, int vehicleWidth,
    int longitudinalAccelerationValue, int longitudinalAccelerationConfidence,
    int curvatureValue, int curvatureConfidence, int curvatureCalculationMode,
    int yawRateValue, int yawRateConfidence);

int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencyAccelerationControl(
    int stationID, uint8_t *buffer, int bufferSize);

int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencyLanePosition(
    int stationID, int lanePosition);

int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(
    int stationID, int steeringWheelAngleValue,
    int steeringWheelAngleConfidence);

int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencyLateralAcceleration(
    int stationID, int lateralAccelerationValue,
    int lateralAccelerationConfidence);

int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(
    int stationID, int verticalAccelerationValue,
    int verticalAccelerationConfidence);

int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencyPerformanceClass(
    int stationID, int performanceClass);

int SHARED_EXPORT setCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(
    int stationID, int protectedZoneLatitude, int protectedZoneLongitude,
    int cenDsrcTollingZoneID);

int SHARED_EXPORT setCAMBasicVehicleContainerLowFrequency(
    int stationID, int vehicleRole, uint8_t *exteriorLights,
    int exteriorLightsSize);

int SHARED_EXPORT addCAMBasicVehicleContainerLowFrequencyPathPoint(
    int stationID, int deltaLatitude, int deltaLongitude, int deltaAltitude,
    int deltaTime);

int SHARED_EXPORT getCAMBasicContainer(
    int stationID, int *stationType, int *latitude, int *longitude,
    int *confidenceMajor, int *confidenceMinor, int *confidenceMajorOrientation,
    int *altitudeValue, int *altitudeConfidence);

int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequency(
    int stationID, int *headingValue, int *headingConfidence, int *speedValue,
    int *speedConfidence, int *driveDirection, int *vehicleLengthValue,
    int *vehicleLengthConfidenceIndication, int *vehicleWidth,
    int *longitudinalAccelerationValue, int *longitudinalAccelerationConfidence,
    int *curvatureValue, int *curvatureConfidence,
    int *curvatureCalculationMode, int *yawRateValue, int *yawRateConfidence);

int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyAccelerationControl(
    int stationID, uint8_t *buffer, int bufferSize, int *actualBufferSize);

int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyLanePosition(
    int stationID, int *lanePosition);

int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(
    int stationID, int *steeringWheelAngleValue,
    int *steeringWheelAngleConfidence);

int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyLateralAcceleration(
    int stationID, int *lateralAccelerationValue,
    int *lateralAccelerationConfidence);

int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(
    int stationID, int *verticalAccelerationValue,
    int *verticalAccelerationConfidence);

int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyPerformanceClass(
    int stationID, int *performanceClass);

int SHARED_EXPORT getCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(
    int stationID, int *protectedZoneLatitude, int *protectedZoneLongitude,
    int *cenDsrcTollingZoneID);

int SHARED_EXPORT getCAMBasicVehicleContainerLowFrequency(
    int stationID, int *vehicleRole, uint8_t *exteriorLights,
    int exteriorLightsSize, int *actualExteriorLightsSize);

int SHARED_EXPORT getCAMBasicVehicleContainerLowFrequencyPathHistory(
    int stationID, int *pathHistory, int pathHistorySize,
    int *actualPathHistorySize);

#ifdef __cplusplus
}
};
#endif