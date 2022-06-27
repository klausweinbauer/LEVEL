#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <c2xcommon.h>

#ifdef __cplusplus
namespace c2x {
extern "C"
{
#endif

int SHARED_EXPORT createDENM(int stationID, int sequenceNumber);
int SHARED_EXPORT deleteDENM(int stationID, int sequenceNumber);

#pragma region Setter
int SHARED_EXPORT setDENMHeader(int stationID, int sequenceNumber, int protocolVersion, int messageID);

int SHARED_EXPORT setDENMManagementContainer(int stationID, int sequenceNumber, int originatingStationID, int detectionTime, int referenceTime,  
    int termination, int relevanceDistance, int relevanceTrafficDirection, int validityDuration, int transmissionInterval, int stationType);
int SHARED_EXPORT setDENMManagementContainerEventPosition(int stationID, int sequenceNumber, int latitude, int longitude,
    int posConfEllSemiMajorConf, int posConfEllSemiMinorConf, int posConfEllSemiMajorOri, int altitudeValue, int altitudeConf);

int SHARED_EXPORT setDENMSituationContainer(int stationID, int sequenceNumber, int informationQuality, int causeCode, int subCauseCode);
int SHARED_EXPORT setDENMSituationContainerLinkedCause(int stationID, int sequenceNumber, int causeCode, int subCauseCode);
int SHARED_EXPORT addDENMSituationContainerEventPoint(int stationID, int sequenceNumber, int deltaLatitude, int deltaLongitude, int deltaAltitude, int deltaTime, int informationQuality);

int SHARED_EXPORT addDENMLocationContainerTrace(int stationID, int sequenceNumber, int* trace, int traceLength);
int SHARED_EXPORT clearDENMLocationContainerTraces(int stationID, int sequenceNumber);
int SHARED_EXPORT setDENMLocationContainerSpeed(int stationID, int sequenceNumber, int speedValue, int speedConfidence);
int SHARED_EXPORT setDENMLocationContainerHeading(int stationID, int sequenceNumber, int headingValue, int headingConfidence);
int SHARED_EXPORT setDENMLocationContainerRoadType(int stationID, int sequenceNumber, int roadType);

int SHARED_EXPORT setDENMAlacarteContainer(int stationID, int sequenceNumber, int lanePosition, int externalTemperature, int positioningSolution);

int SHARED_EXPORT setDENMImpactReductionContainer(int stationID, int sequenceNumber, int heightLonCarrLeft, int heightLonCarrRight, int posLonCarrLeft, 
    int posLonCarrRight, int *positionOfPillars, int positionOfPillarsSize, int posCentMass, int wheelBaseVehicle, int turningRadius, int posFrontAx, 
    uint8_t *positionOfOccupants, int positionOfOccupantsSize, int vehicleMass, int requestResponseIndication);

int SHARED_EXPORT setDENMRoadWorksContainerExtendedLightBarSiren(int stationID, int sequenceNumber, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize);
int SHARED_EXPORT setDENMRoadWorksContainerExtendedClosedLanes(int stationID, int sequenceNumber, int innerhardShoulderStatus, int outerhardShoulderStatus, int drivingLaneStatus);
int SHARED_EXPORT setDENMRoadWorksContainerExtendedRestriction(int stationID, int sequenceNumber, int *stationTypes, int stationTypesSize);
int SHARED_EXPORT setDENMRoadWorksContainerExtendedSpeedLimit(int stationID, int sequenceNumber, int speedLimit);
int SHARED_EXPORT setDENMRoadWorksContainerExtendedIncidentIndication(int stationID, int sequenceNumber, int causeCode, int subCauseCode);
int SHARED_EXPORT setDENMRoadWorksContainerExtendedRecommendedPath(int stationID, int sequenceNumber, int *referencePositions, int referencePositionsSize);
int SHARED_EXPORT setDENMRoadWorksContainerExtendedStartingPointSpeedLimit(int stationID, int sequenceNumber, int deltaLatitude, int deltaLongitude, int deltaAltitude);
int SHARED_EXPORT setDENMRoadWorksContainerExtendedTrafficFlowRule(int stationID, int sequenceNumber, int trafficFlowRule);
int SHARED_EXPORT setDENMRoadWorksContainerExtendedReferenceDenms(int stationID, int sequenceNumber, int *actionIDs, int actionIDsSize);

int SHARED_EXPORT setDENMStationaryVehicleContainerStationarySince(int stationID, int sequenceNumber, int stationarySince);
int SHARED_EXPORT setDENMStationaryVehicleContainerStationaryCause(int stationID, int sequenceNumber, int causeCode, int subCauseCode);
int SHARED_EXPORT setDENMStationaryVehicleContainerCarryingDangerousGoods(int stationID, int sequenceNumber, int dangerousGoodsType, 
    int unNumber, int elevatedTemperature, int tunnelsRestricted, int limitedQuantity);
int SHARED_EXPORT setDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode(int stationID, int sequenceNumber, uint8_t *emergencyActionCode, int emergencyActionCodeSize);
int SHARED_EXPORT setDENMStationaryVehicleContainerCarryingDangerousGoodsPhoneNumber(int stationID, int sequenceNumber, uint8_t *phoneNumber, int phoneNumberSize);
int SHARED_EXPORT setDENMStationaryVehicleContainerCarryingDangerousGoodsCompanyName(int stationID, int sequenceNumber, uint8_t *companyName, int companyNameSize);
int SHARED_EXPORT setDENMStationaryVehicleContainerNumberOfOccupants(int stationID, int sequenceNumber, int numberOfOccupants);
int SHARED_EXPORT setDENMStationaryVehicleContainerVehicleIdentification(int stationID, int sequenceNumber, uint8_t *wMInumber, int wMInumberSize, uint8_t *vDS, int vDSSize);
int SHARED_EXPORT setDENMStationaryVehicleContainerEnergyStorageType(int stationID, int sequenceNumber, int energyStorageType);
#pragma endregion

#pragma region Getter
int SHARED_EXPORT getDENMHeader(int stationID, int sequenceNumber, int *protocolVersion, int *messageID);

int SHARED_EXPORT getDENMManagementContainer(int stationID, int sequenceNumber, int* detectionTime, int* referenceTime, 
    int* termination, int* relevanceDistance, int* relevanceTrafficDirection, int* validityDuration, int* transmissionInterval, int* stationType);
int SHARED_EXPORT getDENMManagementContainerEventPosition(int stationID, int sequenceNumber, int* latitude, int* longitude,
    int* posConfEllSemiMajorConf, int* posConfEllSemiMinorConf, int* posConfEllSemiMajorOri, int* altitudeValue, int* altitudeConf);
    
int SHARED_EXPORT getDENMSituationContainer(int stationID, int sequenceNumber, int *informationQuality, int *causeCode, int *subCauseCode);
int SHARED_EXPORT getDENMSituationContainerLinkedCause(int stationID, int sequenceNumber, int *causeCode, int *subCauseCode);
int SHARED_EXPORT getDENMSituationContainerEventHistory(int stationID, int sequenceNumber, int *eventHistory, int eventHistorySize, int *actualEventHistorySize);

int SHARED_EXPORT getDENMLocationContainerTrace(int stationID, int sequenceNumber, int traceIndex, int* trace, int traceBufferLength, int *actualTraceLength);
int SHARED_EXPORT getDENMLocationContainerSpeed(int stationID, int sequenceNumber, int *speedValue, int *speedConfidence);
int SHARED_EXPORT getDENMLocationContainerHeading(int stationID, int sequenceNumber, int *headingValue, int *headingConfidence);
int SHARED_EXPORT getDENMLocationContainerRoadType(int stationID, int sequenceNumber, int *roadType);

int SHARED_EXPORT getDENMAlacarteContainer(int stationID, int sequenceNumber, int *lanePosition, int *externalTemperature, int *positioningSolution);

int SHARED_EXPORT getDENMImpactReductionContainer(int stationID, int sequenceNumber, int *heightLonCarrLeft, int *heightLonCarrRight, int *posLonCarrLeft, 
    int *posLonCarrRight, int *positionOfPillars, int positionOfPillarsSize, int *posCentMass, int *wheelBaseVehicle, int *turningRadius, int *posFrontAx, 
    uint8_t *positionOfOccupants, int positionOfOccupantsSize, int *vehicleMass, int *requestResponseIndication);

int SHARED_EXPORT getDENMRoadWorksContainerExtendedLightBarSiren(int stationID, int sequenceNumber, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize);
int SHARED_EXPORT getDENMRoadWorksContainerExtendedClosedLanes(int stationID, int sequenceNumber, int *innerhardShoulderStatus, int *outerhardShoulderStatus, int *drivingLaneStatus);
int SHARED_EXPORT getDENMRoadWorksContainerExtendedRestriction(int stationID, int sequenceNumber, int *stationTypes, int stationTypesSize);
int SHARED_EXPORT getDENMRoadWorksContainerExtendedSpeedLimit(int stationID, int sequenceNumber, int *speedLimit);
int SHARED_EXPORT getDENMRoadWorksContainerExtendedIncidentIndication(int stationID, int sequenceNumber, int *causeCode, int *subCauseCode);
int SHARED_EXPORT getDENMRoadWorksContainerExtendedRecommendedPath(int stationID, int sequenceNumber, int *referencePositions, int referencePositionsSize);
int SHARED_EXPORT getDENMRoadWorksContainerExtendedStartingPointSpeedLimit(int stationID, int sequenceNumber, int *deltaLatitude, int *deltaLongitude, int *deltaAltitude);
int SHARED_EXPORT getDENMRoadWorksContainerExtendedTrafficFlowRule(int stationID, int sequenceNumber, int *trafficFlowRule);
int SHARED_EXPORT getDENMRoadWorksContainerExtendedReferenceDenms(int stationID, int sequenceNumber, int *actionIDs, int actionIDsSize);

int SHARED_EXPORT getDENMStationaryVehicleContainerStationarySince(int stationID, int sequenceNumber, int *stationarySince);
int SHARED_EXPORT getDENMStationaryVehicleContainerStationaryCause(int stationID, int sequenceNumber, int *causeCode, int *subCauseCode);
int SHARED_EXPORT getDENMStationaryVehicleContainerCarryingDangerousGoods(int stationID, int sequenceNumber, int *dangerousGoodsType, 
    int *unNumber, int *elevatedTemperature, int *tunnelsRestricted, int *limitedQuantity);
int SHARED_EXPORT getDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode(int stationID, int sequenceNumber, uint8_t *emergencyActionCode, int emergencyActionCodeSize);
int SHARED_EXPORT getDENMStationaryVehicleContainerCarryingDangerousGoodsPhoneNumber(int stationID, int sequenceNumber, uint8_t *phoneNumber, int phoneNumberSize);
int SHARED_EXPORT getDENMStationaryVehicleContainerCarryingDangerousGoodsCompanyName(int stationID, int sequenceNumber, uint8_t *companyName, int companyNameSize);
int SHARED_EXPORT getDENMStationaryVehicleContainerNumberOfOccupants(int stationID, int sequenceNumber, int *numberOfOccupants);
int SHARED_EXPORT getDENMStationaryVehicleContainerVehicleIdentification(int stationID, int sequenceNumber, uint8_t *wMInumber, int wMInumberSize, uint8_t *vDS, int vDSSize);
int SHARED_EXPORT getDENMStationaryVehicleContainerEnergyStorageType(int stationID, int sequenceNumber, int *energyStorageType);
#pragma endregion

#pragma region De-/En-coding
int SHARED_EXPORT encodeDENM(int stationID, int sequenceNumber, uint8_t* buffer, int size, int *actualSize, int encoding);
int SHARED_EXPORT decodeDENM(int* stationID, int* sequenceNumber, uint8_t* buffer, int size, int encoding);
#pragma endregion

#pragma region NetworkService
int SHARED_EXPORT startDENMReceiver(int port, int encoding);
int SHARED_EXPORT stopDENMReceiver();
int SHARED_EXPORT startDENMTransmitter(int port, int encoding);
int SHARED_EXPORT stopDENMTransmitter();
int SHARED_EXPORT setDENMTransmissionFrequency(double f);
int SHARED_EXPORT setDENMTransmissionSource(int stationID, int sequenceNumber);

int SHARED_EXPORT setDENMSendCallback(void (*func)(int stationId, int sequenceNumber));
int SHARED_EXPORT setDENMRecvCallback(void (*func)(int stationId, int sequenceNumber));
#pragma endregion

#ifdef __cplusplus
}
};
#endif