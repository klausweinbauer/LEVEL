#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <c2xcommon.h>

#ifdef _WIN32
#ifdef shared_EXPORTS
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT __declspec(dllimport)
#endif
#else
#define SHARED_EXPORT
#endif

#define _COMMON_DENM

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
#pragma endregion

#pragma region Getter
int SHARED_EXPORT getDENMHeader(int stationID, int sequenceNumber, int *protocolVersion, int *messageID);
int SHARED_EXPORT getDENMManagementContainer(int stationID, int sequenceNumber, int* detectionTime, int* referenceTime, 
    int* termination, int* relevanceDistance, int* relevanceTrafficDirection, int* validityDuration, int* transmissionInterval, int* stationType);
int SHARED_EXPORT getDENMManagementContainerEventPosition(int stationID, int sequenceNumber, int* latitude, int* longitude,
    int* posConfEllSemiMajorConf, int* posConfEllSemiMinorConf, int* posConfEllSemiMajorOri, int* altitudeValue, int* altitudeConf);
int SHARED_EXPORT getDENMSituationContainer(int stationID, int sequenceNumber, int *informationQuality, int *causeCode, int *subCauseCode);
int SHARED_EXPORT getDENMSituationContainerLinkedCause(int stationID, int sequenceNumber, int *causeCode, int *subCauseCode);
int SHARED_EXPORT getDENMSituationContainerEventHistory(int stationIDe, int sequenceNumber, int *eventHistory, int eventHistorySize, int *actualEventHistorySize);
int SHARED_EXPORT getDENMLocationContainerTrace(int stationID, int sequenceNumber, int traceIndex, int* trace, int traceBufferLength, int *actualTraceLength);
int SHARED_EXPORT getDENMLocationContainerSpeed(int stationID, int sequenceNumber, int *speedValue, int *speedConfidence);
int SHARED_EXPORT getDENMLocationContainerHeading(int stationID, int sequenceNumber, int *headingValue, int *headingConfidence);
int SHARED_EXPORT getDENMLocationContainerRoadType(int stationID, int sequenceNumber, int *roadType);
#pragma endregion

#pragma region De-/En-coding
int SHARED_EXPORT encodeDENM(int stationID, int sequenceNumber, uint8_t* buffer, int size, int *actualSize);
int SHARED_EXPORT decodeDENM(int* stationID, int* sequenceNumber, uint8_t* buffer, int size);
#pragma endregion

#pragma region NetworkService
int SHARED_EXPORT startDENMReceiver(int port);
int SHARED_EXPORT stopDENMReceiver();
int SHARED_EXPORT startDENMTransmitter(int port);
int SHARED_EXPORT stopDENMTransmitter();
int SHARED_EXPORT setDENMTransmissionFrequency(double f);
int SHARED_EXPORT setDENMTransmissionSource(int stationID, int sequenceNumber);
#pragma endregion

#ifdef __cplusplus
}
};
#endif