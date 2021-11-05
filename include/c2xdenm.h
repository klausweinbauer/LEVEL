#ifndef __c2xdenm_h
#define __c2xdenm_h

#include <stdint.h>
#include <stdbool.h>

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
extern "C"
{
#endif

//void SHARED_EXPORT initdenm();
int SHARED_EXPORT configureDENM(int port);
void SHARED_EXPORT setDENMTransmissionFrequency(double transmission_frequency);

// send DENM
int SHARED_EXPORT createDENM(int protocolVersion, int messageId, int stationId, int sequenceNumber);
int SHARED_EXPORT terminateDENM(int stationId, int sequenceNumber);
int SHARED_EXPORT setActiveState(int stationId, int sequenceNumber, bool state);

int SHARED_EXPORT setManagementContainer(
    int stationId, int sequenceNumber,
    uint8_t* detectionTime, int detectionTimeSize,
    uint8_t* referenceTime, int referenceTimeSize,
    int termination, int relevanceDistance,
    int relevanceTrafficDirection, int validityDuration,
    int transmissionInterval, int stationType);

int SHARED_EXPORT setManagementContainerEventPosition(
    int stationId, int sequenceNumber,
    int latitude, int intitude,
    int posConfEllSemiMajorConf, int posConfEllSemiMinorConf, int posConfEllSemiMajorOri,
    int altitudeValue, int altitudeConf);

// receive DENM
int SHARED_EXPORT getManagementContainer(
    int stationId, int sequenceNumber,
    uint8_t * detectionTime, int* detectionTimeSize, int detectionTimeMaxSize,
    uint8_t * referenceTime, int* referenceTimeSize, int referenceTimeMaxSize,
    int* termination, int* relevanceDistance,
    int* relevanceTrafficDirection, int* validityDuration,
    int* transmissionInterval, int* stationType);

int SHARED_EXPORT getManagementContainerEventPosition(
    int stationId, int sequenceNumber,
    int* latitude, int* intitude,
    int* posConfEllSemiMajorConf, int* posConfEllSemiMinorConf, int* posConfEllSemiMajorOri,
    int* altitudeValue, int* altitudeConf);


void SHARED_EXPORT startDENMReceiver();
void SHARED_EXPORT stopDENMReceiver();
void SHARED_EXPORT startDENMTransmitter();
void SHARED_EXPORT stopDENMTransmitter();

void SHARED_EXPORT cleanupDENM();

#ifdef __cplusplus
}
#endif

#endif // __c2xdenm_h