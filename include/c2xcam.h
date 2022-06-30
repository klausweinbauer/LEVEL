/**
 * @file c2xcam.h
 * @author Klaus Weinbauer
 * @brief C compatipel API for CAM-Service. Include this file if you want to use
 * CAM-Service.
 * @version 0.1
 * @date 2022-06-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <c2xcam_BasicContainer.h>
#include <c2xcam_config.h>
#include <c2xcam_header.h>
#include <c2xcommon.h>

#ifdef __cplusplus
/**
 * @brief Library namespace
 *
 */
namespace c2x {
extern "C" {
#endif

int SHARED_EXPORT addCAMRSUContainerHighFrequencyProtectedCommunicationZone(
    int stationID, int protectedZoneType, int expiryTime,
    int protectedZoneLatitude, int protectedZoneLongitude,
    int protectedZoneRadius, int protectedZoneID);

int SHARED_EXPORT
clearCAMRSUContainerHighFrequencyProtectedCommunicationZones(int stationID);

int SHARED_EXPORT setCAMPublicTransportContainer(int stationID,
                                                 int embarkationStatus,
                                                 int ptActivationType,
                                                 uint8_t *ptActivationData,
                                                 int ptActivationDataSize);

int SHARED_EXPORT setCAMSpecialTransportContainer(int stationID,
                                                  uint8_t *specialTransportType,
                                                  int specialTransportTypeSize,
                                                  uint8_t *lightBarSirenInUse,
                                                  int lightBarSirenInUseSize);

int SHARED_EXPORT setCAMDangerousGoodsContainer(int stationID,
                                                int dangerousGoodsBasic);

int SHARED_EXPORT setCAMRoadWorksContainerBasic(
    int stationID, int roadworksSubCauseCode, uint8_t *lightBarSirenInUse,
    int lightBarSirenInUseSize, int closedLaneInnerhardShoulderStatus,
    int closedLaneOuterhardShoulderStatus, uint8_t *closedLaneDrivingLaneStatus,
    int closedLaneDrivingLaneStatusSize);

int SHARED_EXPORT setCAMRescueContainer(int stationID,
                                        uint8_t *lightBarSirenInUse,
                                        int lightBarSirenInUseSize);

int SHARED_EXPORT setCAMEmergencyContainer(int stationID,
                                           uint8_t *lightBarSirenInUse,
                                           int lightBarSirenInUseSize,
                                           int causeCode, int subCauseCode,
                                           uint8_t *emergencyPriority,
                                           int emergencyPrioritySize);

int SHARED_EXPORT setCAMSafetyCarContainer(int stationID,
                                           uint8_t *lightBarSirenInUse,
                                           int lightBarSirenInUseSize,
                                           int causeCode, int subCauseCode,
                                           int trafficRule, int speedLimit);

int SHARED_EXPORT getCAMRSUContainerHighFrequencyProtectedCommunicationZone(
    int stationID, int index, int *protectedZoneType, int *expiryTime,
    int *protectedZoneLatitude, int *protectedZoneLongitude,
    int *protectedZoneRadius, int *protectedZoneID);

int SHARED_EXPORT getCAMPublicTransportContainer(int stationID,
                                                 int *embarkationStatus,
                                                 int *ptActivationType,
                                                 uint8_t *ptActivationData,
                                                 int ptActivationDataSize);

int SHARED_EXPORT getCAMSpecialTransportContainer(int stationID,
                                                  uint8_t *specialTransportType,
                                                  int specialTransportTypeSize,
                                                  uint8_t *lightBarSirenInUse,
                                                  int lightBarSirenInUseSize);

int SHARED_EXPORT getCAMDangerousGoodsContainer(int stationID,
                                                int *dangerousGoodsBasic);

int SHARED_EXPORT getCAMRoadWorksContainerBasic(
    int stationID, int *roadworksSubCauseCode, uint8_t *lightBarSirenInUse,
    int lightBarSirenInUseSize, int *closedLaneInnerhardShoulderStatus,
    int *closedLaneOuterhardShoulderStatus,
    uint8_t *closedLaneDrivingLaneStatus, int closedLaneDrivingLaneStatusSize);

int SHARED_EXPORT getCAMRescueContainer(int stationID,
                                        uint8_t *lightBarSirenInUse,
                                        int lightBarSirenInUseSize);

int SHARED_EXPORT getCAMEmergencyContainer(int stationID,
                                           uint8_t *lightBarSirenInUse,
                                           int lightBarSirenInUseSize,
                                           int *causeCode, int *subCauseCode,
                                           uint8_t *emergencyPriority,
                                           int emergencyPrioritySize);

int SHARED_EXPORT getCAMSafetyCarContainer(int stationID,
                                           uint8_t *lightBarSirenInUse,
                                           int lightBarSirenInUseSize,
                                           int *causeCode, int *subCauseCode,
                                           int *trafficRule, int *speedLimit);

int SHARED_EXPORT encodeCAM(int stationID, uint8_t *buffer, int bufferSize,
                            int *actualBufferSize, int encoding);

int SHARED_EXPORT decodeCAM(int *stationID, uint8_t *buffer, int bufferSize,
                            int encoding);

int SHARED_EXPORT startCAMReceiver(int port, int encoding);
int SHARED_EXPORT getCAMReceiverError();
int SHARED_EXPORT stopCAMReceiver();
int SHARED_EXPORT startCAMTransmitter(int port, int encoding);
int SHARED_EXPORT getCAMTransmitterError();
int SHARED_EXPORT stopCAMTransmitter();
int SHARED_EXPORT setCAMTransmissionFrequency(double f);
int SHARED_EXPORT setCAMIDsForTransmission(int *stationIDs_send, int size);

int SHARED_EXPORT setCAMSendCallback(void (*func)(int stationId));
int SHARED_EXPORT setCAMRecvCallback(void (*func)(int stationId));

#ifdef __cplusplus
}
};
#endif