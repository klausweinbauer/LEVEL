#include <CAM.h>
#include <map>
#include <mutex>
#include <c2xcam.h>
#include <VectorBuffer.hpp>
#include <mutex>
#include <iostream>
#include <CAMReceiver.hpp>
#include <CAMTransmitter.hpp>

#ifdef __cplusplus
namespace c2x {
#endif

static std::map<int, CAM_t*> databaseCAM_;
static std::mutex databaseLockCAM_;
static c2x::Buffer* writeCallbackBufferCAM_;
static std::mutex writeCallbackLockCAM_;

std::map<int, CAM_t*>::iterator getCAMiterator(int stationID) {
    for (auto it = databaseCAM_.begin(); it != databaseCAM_.end(); it++) {
        if (it->first == stationID) {
            return it;
        }
    }
    return databaseCAM_.end();
}

CAM_t* getCAM(int stationID) {
    auto it = getCAMiterator(stationID);
    if (it != databaseCAM_.end()) 
    {
        return it->second;
    }
    return nullptr;
}

void setBitString(BIT_STRING_t *bitString, uint8_t* buffer, int size) {
    if (!buffer) {
        return;
    }

    if (!bitString->buf) {
        bitString->buf = (uint8_t*)malloc(size);
    }
    else {
        bitString->buf = (uint8_t*)realloc(bitString->buf, size);
    }
    memcpy(bitString->buf, buffer, size);
    bitString->size = size;
}

void freePathPoint(PathPoint *pathPoint) {
    if (!pathPoint) {
        return;
    }
    delete pathPoint->pathDeltaTime;
    delete pathPoint;
}

int CAMMessage(int stationID, int heighFrequencyContainerType)
{
    return createCAM(stationID, heighFrequencyContainerType);
}

int createCAM(int stationID, int heighFrequencyContainerType) {

    databaseLockCAM_.lock();
    auto it = databaseCAM_.find(stationID);
    if (it != databaseCAM_.end()) {
        databaseLockCAM_.unlock();
        return ERR_CAM_ALREADY_EXISTS;
    }

    CAM_t* cam  = new CAM_t();
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_ALLOC_FAILED;
    }
    cam->header.stationID = stationID;

    switch (heighFrequencyContainerType)
    {
    case HIGH_FREQ_CONTAINER_TYPE_RSU:
        cam->cam.camParameters.highFrequencyContainer.present
            = HighFrequencyContainer_PR::
                HighFrequencyContainer_PR_rsuContainerHighFrequency;
        break;
    default:
        cam->cam.camParameters.highFrequencyContainer.present
            = HighFrequencyContainer_PR::
                HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
        break;
    }

    databaseCAM_.insert(std::pair<int, CAM_t*>(stationID, cam));
    databaseLockCAM_.unlock();

    return stationID;
}

int deleteCAM(int id) {
    databaseLockCAM_.lock();
    auto it = databaseCAM_.find(id);
    if (it == databaseCAM_.end()) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }
    ASN_STRUCT_FREE(asn_DEF_CAM, it->second);
    databaseCAM_.erase(id);

    databaseLockCAM_.unlock();
    return 0;
}

#pragma region Setter

int setCAMHeader(int stationID, int protocolVersion, int messageID) 
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    cam->header.protocolVersion = protocolVersion;
    cam->header.messageID = messageID;
    cam->header.stationID = stationID;

    databaseLockCAM_.unlock();
    return 0;
}

int setCAMGenerationDeltaTime(int stationID, int generationDeltaTime) {
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    cam->cam.generationDeltaTime = generationDeltaTime;

    databaseLockCAM_.unlock();
}

int setCAMBasicContainer(int stationID, int stationType, int latitude, int longitude, int confidenceMajor, 
    int confidenceMinor, int confidenceMajorOrientation, int altitudeValue, int altitudeConfidence) 
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    cam->cam.camParameters.basicContainer.stationType = stationType;
    ReferencePosition *pos = 
        &cam->cam.camParameters.basicContainer.referencePosition;
    pos->latitude = latitude;
    pos->longitude = longitude;
    pos->positionConfidenceEllipse.semiMajorConfidence = confidenceMajor;
    pos->positionConfidenceEllipse.semiMinorConfidence = confidenceMinor;
    pos->positionConfidenceEllipse.semiMajorOrientation = confidenceMajorOrientation;
    pos->altitude.altitudeValue = altitudeValue;
    pos->altitude.altitudeConfidence = altitudeConfidence;

    databaseLockCAM_.unlock();
}

int setCAMBasicVehicleContainerHighFrequency(int stationID,
    int headingValue, int headingConfidence,
    int speedValue, int speedConfidence,
    int driveDirection,
    int vehicleLengthValue, int vehicleLengthConfidenceIndication,
    int vehicleWidth,
    int longitudinalAccelerationValue, int longitudinalAccelerationConfidence,
    int curvatureValue, int curvatureConfidence,
    int curvatureCalculationMode,
    int yawRateValue, int yawRateConfidence)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc =
        &cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency;

    hfc->heading.headingValue = headingValue;
    hfc->heading.headingConfidence = headingConfidence;
    hfc->speed.speedValue = speedValue;
    hfc->speed.speedConfidence = speedConfidence;
    hfc->driveDirection = driveDirection;
    hfc->vehicleLength.vehicleLengthValue = vehicleLengthValue;
    hfc->vehicleLength.vehicleLengthConfidenceIndication = vehicleLengthConfidenceIndication;
    hfc->vehicleWidth = vehicleWidth;
    hfc->longitudinalAcceleration.longitudinalAccelerationValue = longitudinalAccelerationValue;
    hfc->longitudinalAcceleration.longitudinalAccelerationConfidence = longitudinalAccelerationConfidence;
    hfc->curvature.curvatureValue = curvatureValue;
    hfc->curvature.curvatureConfidence = curvatureConfidence;
    hfc->curvatureCalculationMode = curvatureCalculationMode;
    hfc->yawRate.yawRateValue = yawRateValue;
    hfc->yawRate.yawRateConfidence = yawRateConfidence;

    databaseLockCAM_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencyAccelerationControl(int stationID, uint8_t *buffer, int size)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;

    if (!hfc->accelerationControl) {
        hfc->accelerationControl = new AccelerationControl_t();
    }
    setBitString(hfc->accelerationControl, buffer, size);

    databaseLockCAM_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencyLanePosition(int stationID, int lanePosition)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;

    if (!hfc->lanePosition) {
        hfc->lanePosition = new LanePosition_t();
    }
    *hfc->lanePosition = lanePosition;

    databaseLockCAM_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(int stationID, int steeringWheelAngleValue, 
    int steeringWheelAngleConfidence)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;

    if (!hfc->steeringWheelAngle) {
        hfc->steeringWheelAngle = new SteeringWheelAngle_t();
    }
    hfc->steeringWheelAngle->steeringWheelAngleValue = steeringWheelAngleValue;
    hfc->steeringWheelAngle->steeringWheelAngleConfidence = 
        steeringWheelAngleConfidence;

    databaseLockCAM_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencyLateralAcceleration(int stationID, int lateralAccelerationValue, 
    int lateralAccelerationConfidence)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;

    if (!hfc->lateralAcceleration) {
        hfc->lateralAcceleration = new LateralAcceleration_t();
    }
    hfc->lateralAcceleration->lateralAccelerationValue = 
        lateralAccelerationValue;
    hfc->lateralAcceleration->lateralAccelerationConfidence = 
        lateralAccelerationConfidence;

    databaseLockCAM_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(int stationID, int verticalAccelerationValue, 
    int verticalAccelerationConfidence)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;

    if (!hfc->verticalAcceleration) {
        hfc->verticalAcceleration = new VerticalAcceleration();
    }
    hfc->verticalAcceleration->verticalAccelerationValue = 
        verticalAccelerationValue;
    hfc->verticalAcceleration->verticalAccelerationConfidence = 
        verticalAccelerationConfidence;

    databaseLockCAM_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencyPerformanceClass(int stationID, int performanceClass)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;

    if (!hfc->performanceClass) {
        hfc->performanceClass = new PerformanceClass_t();
    }
    *hfc->performanceClass = performanceClass;

    databaseLockCAM_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(int stationID, int protectedZoneLatitude, 
    int protectedZoneLongitude, int cenDsrcTollingZoneID)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }
    
    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;

    if (!hfc->cenDsrcTollingZone) {
        hfc->cenDsrcTollingZone = new CenDsrcTollingZone();
    }
    hfc->cenDsrcTollingZone->protectedZoneLatitude = protectedZoneLatitude;
    hfc->cenDsrcTollingZone->protectedZoneLongitude = 
        protectedZoneLongitude;
    if (!hfc->cenDsrcTollingZone->cenDsrcTollingZoneID) {
        hfc->cenDsrcTollingZone->cenDsrcTollingZoneID = 
            new CenDsrcTollingZoneID_t();
    }
    *hfc->cenDsrcTollingZone->cenDsrcTollingZoneID = cenDsrcTollingZoneID;

    databaseLockCAM_.unlock();
}


int setCAMBasicVehicleContainerLowFrequency(int stationID, int vehicleRole, uint8_t *exteriorLights, int exteriorLightsSize)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }
    
    if (!cam->cam.camParameters.lowFrequencyContainer) 
    {
        cam->cam.camParameters.lowFrequencyContainer = new LowFrequencyContainer();
        cam->cam.camParameters.lowFrequencyContainer->present = 
            LowFrequencyContainer_PR_basicVehicleContainerLowFrequency;
    }
    else if (cam->cam.camParameters.lowFrequencyContainer->present != 
        LowFrequencyContainer_PR_basicVehicleContainerLowFrequency) {
        return ERR_LOW_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerLowFrequency *lfc = &cam->cam.camParameters
        .lowFrequencyContainer->choice.basicVehicleContainerLowFrequency;
    
    lfc->vehicleRole = vehicleRole;
    setBitString(&lfc->exteriorLights, exteriorLights, exteriorLightsSize);

    databaseLockCAM_.unlock();
}

int addCAMBasicVehicleContainerLowFrequencyPathPoint(int stationID, int deltaLatitude, int deltaLongitude, int deltaAltitude, 
    int deltaTime)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }
    
    if (!cam->cam.camParameters.lowFrequencyContainer) 
    {
        cam->cam.camParameters.lowFrequencyContainer = new LowFrequencyContainer();
        cam->cam.camParameters.lowFrequencyContainer->present = 
            LowFrequencyContainer_PR_basicVehicleContainerLowFrequency;
    }
    else if (cam->cam.camParameters.lowFrequencyContainer->present != 
        LowFrequencyContainer_PR_basicVehicleContainerLowFrequency) {
        return ERR_LOW_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerLowFrequency *lfc = &cam->cam.camParameters
        .lowFrequencyContainer->choice.basicVehicleContainerLowFrequency;
    
    PathPoint *pp = new PathPoint();
    pp->pathDeltaTime = new PathDeltaTime_t();
    *pp->pathDeltaTime = deltaTime;
    pp->pathPosition.deltaLatitude = deltaLatitude;
    pp->pathPosition.deltaLongitude = deltaLongitude;
    pp->pathPosition.deltaAltitude = deltaAltitude;
    lfc->pathHistory.list.free = freePathPoint;
    asn_sequence_add(&lfc->pathHistory.list, pp);

    databaseLockCAM_.unlock();
}

#pragma endregion

#pragma region Getter
void getCAMHeader(CAM_t* cam, int *protocolVersion, int *messageID)
{
    if (protocolVersion) {
        *protocolVersion = cam->header.protocolVersion;        
    }
    if (messageID) {
        *messageID = cam->header.messageID;
    }
}

int getCAMHeader(int stationID, int *protocolVersion, int *messageID) 
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    getCAMHeader(cam, protocolVersion, messageID);

    databaseLockCAM_.unlock();
    return 0;
}

void getCAMGenerationDeltaTime(CAM_t* cam, int *generationDeltaTime) 
{
    if (generationDeltaTime) {
        *generationDeltaTime = cam->cam.generationDeltaTime;
    }
}

int getCAMGenerationDeltaTime(int stationID, int *generationDeltaTime) 
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    getCAMGenerationDeltaTime(cam, generationDeltaTime);

    databaseLockCAM_.unlock();
    return 0;
}

void getCAMBasicContainer(CAM_t* cam, int *stationType, int *latitude, int *longitude, int *confidenceMajor, 
    int *confidenceMinor, int *confidenceMajorOrientation, int *altitudeValue, int *altitudeConfidence) 
{
    if (stationType) {
        *stationType = cam->cam.camParameters.basicContainer.stationType;
    }

    ReferencePosition* pos = 
        &cam->cam.camParameters.basicContainer.referencePosition;

    if (latitude) {
        *latitude = pos->latitude;
    }
    if (longitude) {
        *longitude = pos->longitude;
    }
    if (confidenceMajor) {
        *confidenceMajor = pos->positionConfidenceEllipse.semiMajorConfidence;
    } 
    if (confidenceMinor) {
        *confidenceMinor = pos->positionConfidenceEllipse.semiMinorConfidence;
    }
    if (confidenceMajorOrientation) {
        *confidenceMajorOrientation = pos->positionConfidenceEllipse.semiMajorOrientation;
    }
    if (altitudeValue) {
        *altitudeValue = pos->altitude.altitudeValue;
    }
    if (altitudeConfidence) {
        *altitudeConfidence = pos->altitude.altitudeConfidence;
    }
}

int getCAMBasicContainer(int stationID, int *stationType, int *latitude, int *longitude, int *confidenceMajor, 
    int *confidenceMinor, int *confidenceMajorOrientation, int *altitudeValue, int *altitudeConfidence)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    getCAMBasicContainer(cam, stationType, latitude, longitude, confidenceMajor, confidenceMinor, 
        confidenceMajorOrientation, altitudeValue, altitudeConfidence);

    databaseLockCAM_.unlock();
    return 0;
}

int getCAMBasicVehicleContainerHighFrequency(CAM_t *cam, int *headingValue, int *headingConfidence, int *speedValue, 
    int *speedConfidence, int *driveDirection,int *vehicleLengthValue, int *vehicleLengthConfidenceIndication,
    int *vehicleWidth, int *longitudinalAccelerationValue, int *longitudinalAccelerationConfidence,
    int *curvatureValue, int *curvatureConfidence, int *curvatureCalculationMode, int *yawRateValue, 
    int *yawRateConfidence) 
{
    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;

    if (headingValue) {
        *headingValue = hfc->heading.headingValue;
    } 
    if (headingConfidence) {
        *headingConfidence = hfc->heading.headingConfidence;
    } 
    if (speedValue) {
        *speedValue = hfc->speed.speedValue;
    } 
    if (speedConfidence) {
        *speedConfidence = hfc->speed.speedConfidence;
    } 
    if (driveDirection) {
        *driveDirection = hfc->driveDirection;
    } 
    if (vehicleLengthValue) {
        *vehicleLengthValue = hfc->vehicleLength.vehicleLengthValue;
    } 
    if (vehicleLengthConfidenceIndication) {
        *vehicleLengthConfidenceIndication = 
            hfc->vehicleLength.vehicleLengthConfidenceIndication;
    } 
    if (vehicleWidth) {
        *vehicleWidth = hfc->vehicleWidth;
    } 
    if (longitudinalAccelerationValue) {
        *longitudinalAccelerationValue = 
            hfc->longitudinalAcceleration.longitudinalAccelerationValue;
    } 
    if (longitudinalAccelerationConfidence) {
        *longitudinalAccelerationConfidence = 
            hfc->longitudinalAcceleration.longitudinalAccelerationConfidence;
    } 
    if (curvatureValue) {
        *curvatureValue = hfc->curvature.curvatureValue;
    } 
    if (curvatureConfidence) {
        *curvatureConfidence = hfc->curvature.curvatureConfidence;
    } 
    if (curvatureCalculationMode) {
        *curvatureCalculationMode = hfc->curvatureCalculationMode;
    } 
    if (yawRateValue) {
        *yawRateValue = hfc->yawRate.yawRateValue;
    } 
    if (yawRateConfidence) {
        *yawRateConfidence = hfc->yawRate.yawRateConfidence;
    }
}

int getCAMBasicVehicleContainerHighFrequency(int stationID, int *headingValue, int *headingConfidence, int *speedValue, 
    int *speedConfidence, int *driveDirection,int *vehicleLengthValue, int *vehicleLengthConfidenceIndication,
    int *vehicleWidth, int *longitudinalAccelerationValue, int *longitudinalAccelerationConfidence,
    int *curvatureValue, int *curvatureConfidence, int *curvatureCalculationMode, int *yawRateValue, 
    int *yawRateConfidence)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getCAMBasicVehicleContainerHighFrequency(cam, headingValue, headingConfidence, speedValue, 
        speedConfidence, driveDirection, vehicleLengthValue, vehicleLengthConfidenceIndication, vehicleWidth, 
        longitudinalAccelerationValue, longitudinalAccelerationConfidence, curvatureValue, curvatureConfidence, 
        curvatureCalculationMode, yawRateValue, yawRateConfidence);

    databaseLockCAM_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencyAccelerationControl(CAM_t *cam, uint8_t *buffer, int size, int* actualSize)
{
    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;

    if (!hfc->accelerationControl) {
        return ERR_NULL;
    }

    int cpySize = 0;
    if (buffer) {
        cpySize = (std::min)(size, hfc->accelerationControl->size);
        memcpy(buffer, hfc->accelerationControl, cpySize);
    }
    if (actualSize) {
        *actualSize = cpySize;
    }
}

int getCAMBasicVehicleContainerHighFrequencyAccelerationControl(int stationID, uint8_t *buffer, int size, int* actualSize)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getCAMBasicVehicleContainerHighFrequencyAccelerationControl(cam, buffer, size, actualSize);

    databaseLockCAM_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencyLanePosition(CAM_t *cam, int *lanePosition) 
{
    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;
    
    if (!hfc->lanePosition) {
        return ERR_NULL;
    }

    if (lanePosition) {
        *lanePosition = *hfc->lanePosition;
    }
}

int getCAMBasicVehicleContainerHighFrequencyLanePosition(int stationID, int *lanePosition)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getCAMBasicVehicleContainerHighFrequencyLanePosition(cam, lanePosition);

    databaseLockCAM_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(CAM_t *cam,
    int *steeringWheelAngleValue, int *steeringWheelAngleConfidence)
{
    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;
    
    if (!hfc->steeringWheelAngle) {
        return ERR_NULL;
    }

    if (steeringWheelAngleValue) {
        *steeringWheelAngleValue = 
            hfc->steeringWheelAngle->steeringWheelAngleValue;
    }
    if (steeringWheelAngleConfidence) {
        *steeringWheelAngleConfidence = 
            hfc->steeringWheelAngle->steeringWheelAngleConfidence;
    }
}

int getCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(int stationID, int *steeringWheelAngleValue, 
    int *steeringWheelAngleConfidence)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(cam,
        steeringWheelAngleValue, steeringWheelAngleConfidence);

    databaseLockCAM_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencyLateralAcceleration(CAM_t *cam,
    int *lateralAccelerationValue, int *lateralAccelerationConfidence)
{
    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;

    if (!hfc->lateralAcceleration) {
        return ERR_NULL;
    }

    if (lateralAccelerationValue) {
        *lateralAccelerationValue = 
            hfc->lateralAcceleration->lateralAccelerationValue;
    }
    if (lateralAccelerationConfidence) {
        *lateralAccelerationConfidence = 
            hfc->lateralAcceleration->lateralAccelerationConfidence;
    }
}

int getCAMBasicVehicleContainerHighFrequencyLateralAcceleration(int stationID,
    int *lateralAccelerationValue, int *lateralAccelerationConfidence)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getCAMBasicVehicleContainerHighFrequencyLateralAcceleration(cam,
        lateralAccelerationValue, lateralAccelerationConfidence);

    databaseLockCAM_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(CAM_t *cam,
    int *verticalAccelerationValue, int *verticalAccelerationConfidence)
{
    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;
    
    if (!hfc->verticalAcceleration) {
        return ERR_NULL;
    }

    if (verticalAccelerationValue) {
        *verticalAccelerationValue = 
            hfc->verticalAcceleration->verticalAccelerationValue;
    }
    if (verticalAccelerationConfidence) {
        *verticalAccelerationConfidence = 
            hfc->verticalAcceleration->verticalAccelerationConfidence;
    }
}

int getCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(int stationID,
    int *verticalAccelerationValue, int *verticalAccelerationConfidence)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(cam,
        verticalAccelerationValue, verticalAccelerationConfidence);

    databaseLockCAM_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencyPerformanceClass(CAM_t *cam, int *performanceClass)
{
    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;
    
    if (!hfc->performanceClass) {
        return ERR_NULL;
    }

    if (performanceClass) {
        *performanceClass = *hfc->performanceClass;
    }
}

int getCAMBasicVehicleContainerHighFrequencyPerformanceClass(int stationID, int *performanceClass)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getCAMBasicVehicleContainerHighFrequencyPerformanceClass(cam, performanceClass);

    databaseLockCAM_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(CAM_t *cam,
    int *protectedZoneLatitude, int *protectedZoneLongitude, 
    int *cenDsrcTollingZoneID)
{
    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        return ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerHighFrequency *hfc = &cam->cam.camParameters
        .highFrequencyContainer.choice.basicVehicleContainerHighFrequency;
    
    if (!hfc->cenDsrcTollingZone || 
        !hfc->cenDsrcTollingZone->cenDsrcTollingZoneID) {
        return ERR_NULL;
    }

    if (protectedZoneLatitude) {
        *protectedZoneLatitude = hfc->cenDsrcTollingZone->protectedZoneLatitude;
    }
    if (protectedZoneLongitude)  {
        *protectedZoneLongitude = 
            hfc->cenDsrcTollingZone->protectedZoneLongitude;
    }
    if (cenDsrcTollingZoneID) {
        *cenDsrcTollingZoneID = *hfc->cenDsrcTollingZone->cenDsrcTollingZoneID;
    }
}

int getCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(int stationID,
    int *protectedZoneLatitude, int *protectedZoneLongitude, int *cenDsrcTollingZoneID)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(cam,
        protectedZoneLatitude, protectedZoneLongitude, cenDsrcTollingZoneID);

    databaseLockCAM_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerLowFrequency(CAM_t *cam, int *vehicleRole, uint8_t *exteriorLights, 
    int exteriorLightsSize, int *actualExteriorLightsSize)
{
    if (!cam->cam.camParameters.lowFrequencyContainer) {
        return ERR_NULL;
    }

    if (cam->cam.camParameters.lowFrequencyContainer->present != 
        LowFrequencyContainer_PR_basicVehicleContainerLowFrequency) 
    {
        return ERR_LOW_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerLowFrequency *lfc = 
        &cam->cam.camParameters.lowFrequencyContainer->choice.basicVehicleContainerLowFrequency;
    
    if (vehicleRole) {
        *vehicleRole = lfc->vehicleRole;
    }

    int cpySize = 0;
    if (exteriorLights && lfc->exteriorLights.buf) {
        cpySize = (std::min)(exteriorLightsSize, lfc->exteriorLights.size);
        memcpy(exteriorLights, lfc->exteriorLights.buf, cpySize);
    }
    if (actualExteriorLightsSize) {
        *actualExteriorLightsSize = lfc->exteriorLights.size;
    }
}

int getCAMBasicVehicleContainerLowFrequency(int stationID, int *vehicleRole, uint8_t *exteriorLights, 
    int exteriorLightsSize, int *actualExteriorLightsSize)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getCAMBasicVehicleContainerLowFrequency(cam, vehicleRole, exteriorLights, exteriorLightsSize, 
        actualExteriorLightsSize);

    databaseLockCAM_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerLowFrequencyPathHistory(CAM_t *cam, int* pathHistory, 
    int pathHistorySize, int *actualPathHistorySize)
{
    if (!cam->cam.camParameters.lowFrequencyContainer) {
        return ERR_NULL;
    }

    if (cam->cam.camParameters.lowFrequencyContainer->present != 
        LowFrequencyContainer_PR_basicVehicleContainerLowFrequency) 
    {
        return ERR_LOW_FREQ_CONTAINER_TYPE_BASIC_VEHICLE;
    }

    BasicVehicleContainerLowFrequency *lfc = 
        &cam->cam.camParameters.lowFrequencyContainer->choice.basicVehicleContainerLowFrequency;

    if (pathHistory)
    {
        int len = lfc->pathHistory.list.count;
        int cpySize = (std::min)((int)(pathHistorySize / 4), len);
        for (int i = 0; i < cpySize; i++)
        {
            pathHistory[i*4 + 0] = lfc->pathHistory.list.array[len - 1 - i]->pathPosition.deltaLatitude;
            pathHistory[i*4 + 1] = lfc->pathHistory.list.array[len - 1 - i]->pathPosition.deltaLongitude;
            pathHistory[i*4 + 2] = lfc->pathHistory.list.array[len - 1 - i]->pathPosition.deltaAltitude;
            pathHistory[i*4 + 3] = *lfc->pathHistory.list.array[len - 1 - i]->pathDeltaTime;
        }
    }
    if (actualPathHistorySize) {
        *actualPathHistorySize = lfc->pathHistory.list.count * 4;
    }
}

int getCAMBasicVehicleContainerLowFrequencyPathHistory(int stationID, int* pathHistory, 
    int pathHistorySize, int *actualPathHistorySize)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getCAMBasicVehicleContainerLowFrequencyPathHistory(cam, pathHistory, pathHistorySize, 
        actualPathHistorySize);

    databaseLockCAM_.unlock();
    return ret;
}
#pragma endregion


#pragma region De-/En-coding
int decodeCAM(int *stationID, uint8_t* buffer, int size)
{
    databaseLockCAM_.lock();

    CAM_t* cam = nullptr;
    asn_dec_rval_t retVal;
    asn_codec_ctx_t opt_codec_ctx{};
    opt_codec_ctx.max_stack_size = 0;
    retVal = xer_decode(&opt_codec_ctx, &asn_DEF_CAM, (void**)&cam, buffer, size);

    if (retVal.code == asn_dec_rval_code_e::RC_FAIL)
    {
        databaseLockCAM_.unlock();
        return ERR_DECODE;
    }

    *stationID = cam->header.stationID;

    if (cam->cam.camParameters.lowFrequencyContainer) {
        LowFrequencyContainer* lfc = cam->cam.camParameters.lowFrequencyContainer;
        if (lfc->present == LowFrequencyContainer_PR_basicVehicleContainerLowFrequency) {
            lfc->choice.basicVehicleContainerLowFrequency.pathHistory.list.free = freePathPoint;
        } 
    }

    auto dbIt = getCAMiterator(*stationID);
    if (dbIt != databaseCAM_.end()) {
        ASN_STRUCT_FREE(asn_DEF_CAM, dbIt->second);
        dbIt->second = cam;
    }
    else {
        databaseCAM_.insert(std::pair<int, CAM_t*>(*stationID, cam));
    }
    databaseLockCAM_.unlock();

    return retVal.code;
}

int writeCallbackCAM(const void* src, size_t size, void* application_specific_key)
{
    return (int)writeCallbackBufferCAM_->write(src, (int)size, application_specific_key);
}

int encodeCAM(int stationID, uint8_t* buffer, int size, int *actualSize)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }
    writeCallbackLockCAM_.lock();
    VectorBuffer* vectorBuffer = new VectorBuffer();
    writeCallbackBufferCAM_ = vectorBuffer;
    asn_enc_rval_t retVal = xer_encode(&asn_DEF_CAM, (void*)cam, XER_F_BASIC, writeCallbackCAM, NULL);
    writeCallbackBufferCAM_ = nullptr;
    writeCallbackLockCAM_.unlock();
    databaseLockCAM_.unlock();

    if (retVal.encoded == -1)
    {
        delete vectorBuffer;
        std::cout << "[ERROR] Code: " << retVal.failed_type->name << " " << retVal.failed_type->xml_tag << std::endl;
        return ERR_ENCODE;
    }

    size_t required_buffer_size = vectorBuffer->size();
    if (size < required_buffer_size)
    {
        delete vectorBuffer;
        return ERR_BUFFER_OVERFLOW;
    }

    ((char*)buffer)[required_buffer_size + 1] = '\0';
    size_t copiedBytes = vectorBuffer->copy(buffer, required_buffer_size);
    if (actualSize) {
        *actualSize = copiedBytes;
    }
    delete vectorBuffer;

    return copiedBytes;
}
#pragma endregion

#pragma region NetworkService
int startCAMReceiver(int port)
{
    try
    {
        CAMReceiver::getInstance().start(port);
    }
    catch (const std::exception& ex)
    {
        return ERR_RECEIVER_START;
        std::cout << "[ERROR] " << ex.what() << std::endl;
    }
    return 0;
}

int stopCAMReceiver()
{
    CAMReceiver::getInstance().stop();
    return 0;
}

int startCAMTransmitter(int port)
{
    try
    {
        CAMTransmitter::getInstance().start(port);
    }
    catch (const std::exception& ex)
    {
        return ERR_TRANSMITTER_START;
        std::cout << "[ERROR] " << ex.what() << std::endl;
    }
    return 0;
}

int stopCAMTransmitter()
{
    CAMTransmitter::getInstance().stop();
    return 0;
}

int setCAMTransmissionFrequency(double f)
{
    unsigned int int_ms = (unsigned int)(1000 / f);
    CAMTransmitter::getInstance().setInterval(int_ms);
    return 0;
}

int CAMTransmitter(int *stationIDs_send, int size)
{
    return CAMTransmitter::getInstance().setIDsToTransmit(stationIDs_send, size);
}

#pragma endregion

#ifdef __cplusplus
};
#endif