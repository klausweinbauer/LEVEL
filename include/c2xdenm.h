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
int SHARED_EXPORT SIMULINK_BLOCK setDENMHeader(int stationID, int sequenceNumber, int protocolVersion, int messageID);
int SHARED_EXPORT SIMULINK_BLOCK setDENMManagementContainer(int stationID, int sequenceNumber, int originatingStationID,
    uint8_t* detectionTime, int detectionTimeSize, uint8_t* referenceTime, int referenceTimeSize, int termination, 
    int relevanceDistance, int relevanceTrafficDirection, int validityDuration, int transmissionInterval, int stationType);
int SHARED_EXPORT SIMULINK_BLOCK setDENMManagementContainerEventPosition(int stationID, int sequenceNumber, int latitude, int longitude,
    int posConfEllSemiMajorConf, int posConfEllSemiMinorConf, int posConfEllSemiMajorOri, int altitudeValue, int altitudeConf);
#pragma endregion

#pragma region Getter
int SHARED_EXPORT SIMULINK_BLOCK getDENMHeader(int stationID, int sequenceNumber, int *protocolVersion, int *messageID);
int SHARED_EXPORT getDENMManagementContainer(int stationID, int sequenceNumber, 
    uint8_t * detectionTime, int detectionTimeSize, int* detectionTimeActualSize,
    uint8_t * referenceTime, int referenceTimeSize, int* referenceTimeActualSize, 
    int* termination, int* relevanceDistance, int* relevanceTrafficDirection, int* validityDuration, int* transmissionInterval, int* stationType);
int SHARED_EXPORT getDENMManagementContainerEventPosition(int stationID, int sequenceNumber, int* latitude, int* longitude,
    int* posConfEllSemiMajorConf, int* posConfEllSemiMinorConf, int* posConfEllSemiMajorOri, int* altitudeValue, int* altitudeConf);
#pragma endregion

#pragma region De-/En-coding
int SHARED_EXPORT SIMULINK_BLOCK encodeDENM(int stationID, int sequenceNumber, uint8_t* buffer, SIMULINK_NONTUN_PROP int size, int *actualSize);
int SHARED_EXPORT SIMULINK_BLOCK decodeDENM(int* stationID, int* sequenceNumber, uint8_t* buffer, int size);
#pragma endregion

#pragma region NetworkService
int SHARED_EXPORT startDENMReceiver(int port);
int SHARED_EXPORT stopDENMReceiver();
int SHARED_EXPORT startDENMTransmitter(int port);
int SHARED_EXPORT stopDENMTransmitter();
int SHARED_EXPORT setDENMTransmissionFrequency(double f);
int SHARED_EXPORT SIMULINK_BLOCK DENMTransmitter(int *stationIDs_send, int size);
#pragma endregion

#ifdef __cplusplus
}
};
#endif