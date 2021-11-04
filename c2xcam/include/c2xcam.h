#ifndef __c2xcam_h
#define __c2xcam_h

#ifdef _WIN32
#ifdef shared_EXPORTS
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT __declspec(dllimport)
#endif
#else
#define SHARED_EXPORT
#endif

#include <stdint.h>

//extern "C" void SHARED_EXPORT initCAM();
extern "C" void SHARED_EXPORT configureCAM(int port, int cam_HFC_present, int cam_LFC_present, int cam_SVC_present);
extern "C" void SHARED_EXPORT setCAMTransmissionFrequency(double transmission_frequency);
extern "C" void SHARED_EXPORT setCAMLFCBasicVehicleMaxPathPoints(int max_points);
extern "C" int SHARED_EXPORT getCAMHeader(int id, int *version, int *msgID, int *stationID);
extern "C" int SHARED_EXPORT getCAMBasicContainerValues(int id, int* stationType, int* latitude, int* longitude,
	int* posConfSemiMajorC, int* posConfSemiMinorC, int* posConfSemiMajorO, int* altitudeValue, int* altitudeConf);
extern "C" int SHARED_EXPORT getCAMHFCBasicVehicleValues(
	int stationId,
	int* headingValue, int* headingConfidence,
	int* speedValue, int* speedConfidence,
	int* driveDirection,
	int* vehicleLengthValue, int* vehicleLengthConfidence,
	int* vehicleWidth,
	int* longitudinalAccelerationValue, int* longitudinalAccelerationConfidence,
	int* curvatureValue, int* curvatureConfidence,
	int* curvatureCalculationMode,
	int* yawRateValue, int* yawRateConfidence);
extern "C" int SHARED_EXPORT getCAMLFCBasicVehicleValues(
	int stationId, 
	int* vehicleRole, 
	uint8_t * exteriorLights, int* exteriorLightsSize, 
	int* pathHistory, int* pathHistorySize);
extern "C" void SHARED_EXPORT setCAMHeader(int version, int msgID, int stationID);
extern "C" void SHARED_EXPORT setCAMBasicContainerValues(int stationType, int latitude, int longitude,
	int posConfSemiMajorC, int posConfSemiMinorC, int posConfSemiMajorO, int altitudeValue, int altitudeConf);
extern "C" void SHARED_EXPORT setCAMHFCBasicVehicleValues(
	int headingValue, int headingConfidence,
	int speedValue, int speedConfidence,
	int driveDirection,
	int vehicleLengthValue, int vehicleLengthConfidence,
	int vehicleWidth,
	int longitudinalAccelerationValue, int longitudinalAccelerationConfidence,
	int curvatureValue, int curvatureConfidence,
	int curvatureCalculationMode,
	int yawRateValue, int yawRateConfidence);
extern "C" void SHARED_EXPORT setCAMLFCBasicVehicleValues(
	int vehicleRole, 
	uint8_t * exteriorLights, int exteriorLightsSize);
extern "C" int SHARED_EXPORT addCAMLFCBasicVehiclePathPoint(int deltaLatitude, int deltaLongitude, 
	int deltaAltitude, int deltaTime);
extern "C" void SHARED_EXPORT startCAMReceiver();
extern "C" void SHARED_EXPORT stopCAMReceiver();
extern "C" void SHARED_EXPORT startCAMTransmitter();
extern "C" void SHARED_EXPORT stopCAMTransmitter();
extern "C" void SHARED_EXPORT cleanup();

#endif // __c2xcam_h