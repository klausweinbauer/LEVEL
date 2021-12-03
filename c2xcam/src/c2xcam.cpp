#include <CAM.h>
#include <map>
#include <mutex>
#include <c2xcam.h>
#include <VectorBuffer.hpp>
#include <mutex>
#include <iostream>
#include <CAMReceiver.hpp>
#include <CAMTransmitter.hpp>
#include <string.h>
#include <sstream>

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

void setBitString(BIT_STRING_t *bitString, uint8_t *data, int dataSize) {
    if (!bitString || !data) {
        return;
    }

    if (!bitString->buf) {
        bitString->buf = (uint8_t*)malloc(dataSize);
    }
    else {
        bitString->buf = (uint8_t*)realloc(bitString->buf, dataSize);
    }
    memcpy(bitString->buf, data, dataSize);
    bitString->size = dataSize;
    bitString->bits_unused = 0;
}

int getBitString(BIT_STRING_t *bitString, uint8_t *buffer, int bufferSize) {
    if (!bitString || !buffer || !bitString->buf) {
        return 0;
    }

    int cpyLen = (std::min)(bufferSize, bitString->size);
    memcpy(buffer, bitString->buf, cpyLen);
    return cpyLen;
}

void setOctetString(OCTET_STRING_t *octetString, uint8_t *data, int dataSize) {
    if (!octetString || !data) {
        return;
    }

    if (!octetString->buf) {
        octetString->buf = (uint8_t*)malloc(dataSize);
    }
    else {
        octetString->buf = (uint8_t*)realloc(octetString->buf, dataSize);
    }
    memcpy(octetString->buf, data, dataSize);
    octetString->size = dataSize;
}

int getOctetString(OCTET_STRING_t *octetString, uint8_t *buffer, int bufferSize) {
    if (!octetString || !buffer || !octetString->buf) {
        return 0;
    }

    int cpyLen = (std::min)(bufferSize, octetString->size);
    memcpy(buffer, octetString->buf, cpyLen);
    return cpyLen;
}

void setTimestamp(TimestampIts_t *timestamp, int time) {
    if (!timestamp) {
        return;
    }

    if (!timestamp->buf) {
        timestamp->buf = (uint8_t*)malloc(sizeof(int));
    }
    else if (timestamp->size != 4) {
        timestamp->buf = (uint8_t*)realloc(timestamp->buf, sizeof(int));
    }
    if (!timestamp->buf)
    {
        return;
    }
    memcpy(timestamp->buf, (uint8_t*)&time, sizeof(int));
    timestamp->size = sizeof(int);
}

void getTimestamp(TimestampIts_t *timestamp, int *time) {
    if (!timestamp || !time) {
        return;
    }

    if (timestamp->buf) {
        int cpySize = (std::min)((int)sizeof(int), timestamp->size);
        memcpy((uint8_t*)time, timestamp->buf, cpySize);
    }
}

void freePathPoint(PathPoint *pathPoint) {
    if (!pathPoint) {
        return;
    }
    delete pathPoint->pathDeltaTime;
    delete pathPoint;
}

void freeProtectedCommunicationZone(ProtectedCommunicationZone *zone) {
    if (!zone) {
        return;
    }
    delete zone->expiryTime;
    delete zone->protectedZoneRadius;
    delete zone->protectedZoneID;
    delete zone;
}

inline const char* toString(SpecialVehicleContainer_PR v)
{
    switch (v)
    {
        case SpecialVehicleContainer_PR_dangerousGoodsContainer: 
            return "DangerousGoodsContainer";
        case SpecialVehicleContainer_PR_emergencyContainer:   
            return "EmergencyContainer";
        case SpecialVehicleContainer_PR_NOTHING:   
            return "[Not present]";
        case SpecialVehicleContainer_PR_publicTransportContainer:
            return "PublicTransportContainer";
        case SpecialVehicleContainer_PR_rescueContainer:
            return "RescueContainer";
        case SpecialVehicleContainer_PR_roadWorksContainerBasic:
            return "RoadWorksContainerBasic";
        case SpecialVehicleContainer_PR_safetyCarContainer:
            return "SafetyCarContainer";
        case SpecialVehicleContainer_PR_specialTransportContainer:
            return "SpecialTransportContainer";
        default:  
            return "[Unknown SpecialVehicleContainer]";
    }
}

int createCAM(int stationID) {

    databaseLockCAM_.lock();
    auto it = databaseCAM_.find(stationID);
    if (it != databaseCAM_.end()) {
        databaseLockCAM_.unlock();
        std::stringstream errMsgStream;
        errMsgStream << "[ERROR] CAM with Id '" << stationID << "' already exists. " 
            << "You can not have two CAMMessage Blocks with the same Id." << std::endl;
        setLastErrMsg(errMsgStream.str().c_str(), errMsgStream.str().size());
        return ERR_CAM_ALREADY_EXISTS;
    }

    CAM_t* cam  = new CAM_t();
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_ALLOC_FAILED;
    }
    cam->header.stationID = stationID;
    cam->cam.camParameters.highFrequencyContainer.present = HighFrequencyContainer_PR_NOTHING;

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

int getBasicVehicleContainerHighFrequency(int stationID, BasicVehicleContainerHighFrequency_t **container, bool initialize = false)
{
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        return ERR_MSG_NOT_FOUND;
    }

    if (cam->cam.camParameters.highFrequencyContainer.present == HighFrequencyContainer_PR_NOTHING) 
    {
        cam->cam.camParameters.highFrequencyContainer.present = HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
    }
    else if (cam->cam.camParameters.highFrequencyContainer.present != HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) 
    {
        std::stringstream errMsgStream;
        errMsgStream << "[ERROR] Wrong type of HighFrequencyContainer. HighFrequencyContainer of CAMMessage from station '" << stationID 
            << "' was already specified as 'RSUContainerHighFrequency'. You can not use multiple HighFrequencyContainers." << std::endl;
        setLastErrMsg(errMsgStream.str().c_str(), errMsgStream.str().size());
        return ERR_HIGH_FREQ_CONTAINER_TYPE;
    }

    *container = &cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency;
    return 0;
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
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

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

int setCAMBasicVehicleContainerHighFrequencyAccelerationControl(int stationID, uint8_t *buffer, int bufferSize)
{
    databaseLockCAM_.lock();
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (!hfc->accelerationControl) {
        hfc->accelerationControl = new AccelerationControl_t();
    }
    setBitString(hfc->accelerationControl, buffer, bufferSize);

    databaseLockCAM_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencyLanePosition(int stationID, int lanePosition)
{
    databaseLockCAM_.lock();
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

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
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

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
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

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
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

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
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

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
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

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
        return ERR_LOW_FREQ_CONTAINER_TYPE;
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
        return ERR_LOW_FREQ_CONTAINER_TYPE;
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

int addCAMRSUContainerHighFrequencyProtectedCommunicationZone(int stationID, int protectedZoneType, 
    int expiryTime, int protectedZoneLatitude, int protectedZoneLongitude, int protectedZoneRadius, int protectedZoneID)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (cam->cam.camParameters.highFrequencyContainer.present == HighFrequencyContainer_PR_NOTHING) {
        cam->cam.camParameters.highFrequencyContainer.present = HighFrequencyContainer_PR_rsuContainerHighFrequency;
    }
    else if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_rsuContainerHighFrequency) 
    {
        databaseLockCAM_.unlock();
        return ERR_HIGH_FREQ_CONTAINER_TYPE;
    }

    ProtectedCommunicationZonesRSU *zonesRSU = cam->cam.camParameters.highFrequencyContainer.choice
        .rsuContainerHighFrequency.protectedCommunicationZonesRSU;
    if (!zonesRSU) {
        zonesRSU = new ProtectedCommunicationZonesRSU();
        cam->cam.camParameters.highFrequencyContainer.choice
        .rsuContainerHighFrequency.protectedCommunicationZonesRSU = zonesRSU;
        zonesRSU->list.free = freeProtectedCommunicationZone;
    }
    
    ProtectedCommunicationZone *zone = new ProtectedCommunicationZone();
    zone->expiryTime = new TimestampIts_t();
    zone->protectedZoneRadius = new ProtectedZoneRadius_t();
    zone->protectedZoneID = new ProtectedZoneID_t;

    setTimestamp(zone->expiryTime, expiryTime);
    zone->protectedZoneType = protectedZoneType;
    zone->protectedZoneLatitude = protectedZoneLatitude;
    zone->protectedZoneLongitude = protectedZoneLongitude;
    *zone->protectedZoneRadius = protectedZoneRadius;
    *zone->protectedZoneID = protectedZoneID;
    asn_sequence_add(zonesRSU, zone);

    databaseLockCAM_.unlock();
    return 0;
}

int clearCAMRSUContainerHighFrequencyProtectedCommunicationZones(int stationID)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (cam->cam.camParameters.highFrequencyContainer.present == HighFrequencyContainer_PR_NOTHING) {
        cam->cam.camParameters.highFrequencyContainer.present = HighFrequencyContainer_PR_rsuContainerHighFrequency;
    }
    else if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_rsuContainerHighFrequency) 
    {
        databaseLockCAM_.unlock();
        return ERR_HIGH_FREQ_CONTAINER_TYPE;
    }

    ProtectedCommunicationZonesRSU *zonesRSU = cam->cam.camParameters.highFrequencyContainer.choice
        .rsuContainerHighFrequency.protectedCommunicationZonesRSU;
    if (!zonesRSU) {
        databaseLockCAM_.unlock();
        return ERR_NULL;
    }
    
    asn_sequence_empty(zonesRSU);

    databaseLockCAM_.unlock();
    return 0;
}

int getSpecialVehicleContainer(int stationID, void **container, SpecialVehicleContainer_PR type, bool initialize = false)
{
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        return ERR_MSG_NOT_FOUND;
    }

    if (!cam->cam.camParameters.specialVehicleContainer) {
        if (initialize) 
        {
            cam->cam.camParameters.specialVehicleContainer = new SpecialVehicleContainer();
            cam->cam.camParameters.specialVehicleContainer->present = type;
        } else {
            return ERR_NULL;
        }
    }

    if (cam->cam.camParameters.specialVehicleContainer->present == type) {
        *container = &cam->cam.camParameters.specialVehicleContainer->choice;
    } else {
        std::stringstream errMsgStream;
        errMsgStream << "[ERROR] Wrong type of SpecialVehicleContainer. You can only have one type of SpecialVehicleContainer per CAM message. "
            << "Container is of type '" << toString(cam->cam.camParameters.specialVehicleContainer->present) << "' "
            << "but type '" << toString(type) << "' "
            << "is needed." << std::endl;
        setLastErrMsg(errMsgStream.str().c_str(), errMsgStream.str().size());
        return ERR_SPECIAL_VEHICLE_CONTAINER_TYPE;
    }
    return 0;
}

template<typename T> int getSpecialVehicleContainer(int stationID, T** container, bool initialize = false);
template<> int getSpecialVehicleContainer<PublicTransportContainer>(int stationID, PublicTransportContainer** container, bool initialize) {
    return getSpecialVehicleContainer(stationID, (void**)container, SpecialVehicleContainer_PR_publicTransportContainer, initialize);
}
template<> int getSpecialVehicleContainer<SpecialTransportContainer>(int stationID, SpecialTransportContainer** container, bool initialize) {
    return getSpecialVehicleContainer(stationID, (void**)container, SpecialVehicleContainer_PR_specialTransportContainer, initialize);
}
template<> int getSpecialVehicleContainer<DangerousGoodsContainer>(int stationID, DangerousGoodsContainer** container, bool initialize) {
    return getSpecialVehicleContainer(stationID, (void**)container, SpecialVehicleContainer_PR_dangerousGoodsContainer, initialize);
}
template<> int getSpecialVehicleContainer<RoadWorksContainerBasic>(int stationID, RoadWorksContainerBasic** container, bool initialize) {
    return getSpecialVehicleContainer(stationID, (void**)container, SpecialVehicleContainer_PR_roadWorksContainerBasic, initialize);
}
template<> int getSpecialVehicleContainer<RescueContainer>(int stationID, RescueContainer** container, bool initialize) {
    return getSpecialVehicleContainer(stationID, (void**)container, SpecialVehicleContainer_PR_rescueContainer, initialize);
}
template<> int getSpecialVehicleContainer<EmergencyContainer>(int stationID, EmergencyContainer** container, bool initialize) {
    return getSpecialVehicleContainer(stationID, (void**)container, SpecialVehicleContainer_PR_emergencyContainer, initialize);
}
template<> int getSpecialVehicleContainer<SafetyCarContainer>(int stationID, SafetyCarContainer** container, bool initialize) {
    return getSpecialVehicleContainer(stationID, (void**)container, SpecialVehicleContainer_PR_safetyCarContainer, initialize);
}


int setCAMPublicTransportContainer(int stationID, int embarkationStatus, int ptActivationType, uint8_t *ptActivationData, int ptActivationDataSize) 
{
    databaseLockCAM_.lock();
    PublicTransportContainer *c;
    int err = getSpecialVehicleContainer(stationID, &c, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (!c->ptActivation) {
        c->ptActivation = new PtActivation_t();
    }

    c->embarkationStatus = embarkationStatus;
    c->ptActivation->ptActivationType = ptActivationType;
    setOctetString(&c->ptActivation->ptActivationData, ptActivationData, ptActivationDataSize);

    databaseLockCAM_.unlock();
    return 0;
}

int setCAMSpecialTransportContainer(int stationID, uint8_t *specialTransportType, int specialTransportTypeSize, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize) 
{
    databaseLockCAM_.lock();
    SpecialTransportContainer *c;
    int err = getSpecialVehicleContainer(stationID, &c, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    setBitString(&c->specialTransportType, specialTransportType, specialTransportTypeSize);
    setBitString(&c->lightBarSirenInUse, lightBarSirenInUse, lightBarSirenInUseSize);

    databaseLockCAM_.unlock();
    return 0;
}

int setCAMDangerousGoodsContainer(int stationID, int dangerousGoodsBasic) 
{
    databaseLockCAM_.lock();
    DangerousGoodsContainer *c;
    int err = getSpecialVehicleContainer(stationID, &c, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    c->dangerousGoodsBasic = dangerousGoodsBasic;

    databaseLockCAM_.unlock();
    return 0;
}

int setCAMRoadWorksContainerBasic(int stationID, int roadworksSubCauseCode, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize, 
    int closedLaneInnerhardShoulderStatus, int closedLaneOuterhardShoulderStatus, uint8_t *closedLaneDrivingLaneStatus, int closedLaneDrivingLaneStatusSize) 
{
    databaseLockCAM_.lock();
    RoadWorksContainerBasic *c;
    int err = getSpecialVehicleContainer(stationID, &c, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (!c->roadworksSubCauseCode) {
        c->roadworksSubCauseCode = new RoadworksSubCauseCode_t();
    }
    if (!c->closedLanes) {
        c->closedLanes = new ClosedLanes();
        c->closedLanes->innerhardShoulderStatus = new HardShoulderStatus_t();
        c->closedLanes->outerhardShoulderStatus = new HardShoulderStatus_t();
        c->closedLanes->drivingLaneStatus = new DrivingLaneStatus_t();
    }

    *c->roadworksSubCauseCode = roadworksSubCauseCode;
    setBitString(&c->lightBarSirenInUse, lightBarSirenInUse, lightBarSirenInUseSize);
    *c->closedLanes->innerhardShoulderStatus = closedLaneInnerhardShoulderStatus;
    *c->closedLanes->outerhardShoulderStatus = closedLaneOuterhardShoulderStatus;
    setBitString(c->closedLanes->drivingLaneStatus, closedLaneDrivingLaneStatus, closedLaneDrivingLaneStatusSize);

    databaseLockCAM_.unlock();
    return 0;
}

int setCAMRescueContainer(int stationID, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize) 
{
    databaseLockCAM_.lock();
    RescueContainer *c;
    int err = getSpecialVehicleContainer(stationID, &c, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    setBitString(&c->lightBarSirenInUse, lightBarSirenInUse, lightBarSirenInUseSize);
    
    databaseLockCAM_.unlock();
    return 0;
}

int setCAMEmergencyContainer(int stationID, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize, int causeCode, 
    int subCauseCode, uint8_t *emergencyPriority, int emergencyPrioritySize) 
{
    databaseLockCAM_.lock();
    EmergencyContainer *c;
    int err = getSpecialVehicleContainer(stationID, &c, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (!c->incidentIndication) {
        c->incidentIndication = new CauseCode();
    }
    if (!c->emergencyPriority) {
        c->emergencyPriority = new EmergencyPriority_t();
    }

    setBitString(&c->lightBarSirenInUse, lightBarSirenInUse, lightBarSirenInUseSize);
    c->incidentIndication->causeCode = causeCode;
    c->incidentIndication->subCauseCode = subCauseCode;
    setBitString(c->emergencyPriority, emergencyPriority, emergencyPrioritySize);
    
    databaseLockCAM_.unlock();
    return 0;
}

int setCAMSafetyCarContainer(int stationID, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize, int causeCode, int subCauseCode, 
    int trafficRule, int speedLimit) 
{
    databaseLockCAM_.lock();
    SafetyCarContainer *c;
    int err = getSpecialVehicleContainer(stationID, &c, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (!c->incidentIndication) {
        c->incidentIndication = new CauseCode();
    }
    if (!c->trafficRule) {
        c->trafficRule = new TrafficRule_t();
    }
    if (!c->speedLimit) {
        c->speedLimit = new SpeedLimit_t();
    }

    setBitString(&c->lightBarSirenInUse, lightBarSirenInUse, lightBarSirenInUseSize);
    c->incidentIndication->causeCode = causeCode;
    c->incidentIndication->subCauseCode = subCauseCode;
    *c->trafficRule = trafficRule;
    *c->speedLimit = speedLimit;
    
    databaseLockCAM_.unlock();
    return 0;
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

int getCAMBasicVehicleContainerHighFrequency(int stationID, int *headingValue, int *headingConfidence, int *speedValue, 
    int *speedConfidence, int *driveDirection,int *vehicleLengthValue, int *vehicleLengthConfidenceIndication,
    int *vehicleWidth, int *longitudinalAccelerationValue, int *longitudinalAccelerationConfidence,
    int *curvatureValue, int *curvatureConfidence, int *curvatureCalculationMode, int *yawRateValue, 
    int *yawRateConfidence)
{
    databaseLockCAM_.lock();
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

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

    databaseLockCAM_.unlock();
    return 0;
}

int getCAMBasicVehicleContainerHighFrequencyAccelerationControl(int stationID, uint8_t *buffer, int bufferSize, int* actualBufferSize)
{
    databaseLockCAM_.lock();
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (!hfc->accelerationControl) {
        databaseLockCAM_.unlock();
        return ERR_NULL;
    }

    int actualSize = getBitString(hfc->accelerationControl, buffer, bufferSize);
    if (actualBufferSize) {
        *actualBufferSize = actualSize;
    }

    databaseLockCAM_.unlock();
    return 0;
}

int getCAMBasicVehicleContainerHighFrequencyLanePosition(int stationID, int *lanePosition)
{
    databaseLockCAM_.lock();
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (!hfc->lanePosition) {
        databaseLockCAM_.unlock();
        return ERR_NULL;
    }

    if (lanePosition) {
        *lanePosition = *hfc->lanePosition;
    }

    databaseLockCAM_.unlock();
    return 0;
}

int getCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(int stationID, int *steeringWheelAngleValue, 
    int *steeringWheelAngleConfidence)
{
    databaseLockCAM_.lock();
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (!hfc->steeringWheelAngle) {
        databaseLockCAM_.unlock();
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
    databaseLockCAM_.unlock();
    return 0;
}

int getCAMBasicVehicleContainerHighFrequencyLateralAcceleration(int stationID,
    int *lateralAccelerationValue, int *lateralAccelerationConfidence)
{
    databaseLockCAM_.lock();
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (!hfc->lateralAcceleration) {
        databaseLockCAM_.unlock();
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

    databaseLockCAM_.unlock();
    return 0;
}

int getCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(int stationID,
    int *verticalAccelerationValue, int *verticalAccelerationConfidence)
{
    databaseLockCAM_.lock();
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (!hfc->verticalAcceleration) {
        databaseLockCAM_.unlock();
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

    databaseLockCAM_.unlock();
    return 0;
}

int getCAMBasicVehicleContainerHighFrequencyPerformanceClass(int stationID, int *performanceClass)
{
    databaseLockCAM_.lock();
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (!hfc->performanceClass) {
        databaseLockCAM_.unlock();
        return ERR_NULL;
    }

    if (performanceClass) {
        *performanceClass = *hfc->performanceClass;
    }

    databaseLockCAM_.unlock();
    return 0;
}

int getCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(int stationID,
    int *protectedZoneLatitude, int *protectedZoneLongitude, int *cenDsrcTollingZoneID)
{
    databaseLockCAM_.lock();
    BasicVehicleContainerHighFrequency *hfc;
    int err = getBasicVehicleContainerHighFrequency(stationID, &hfc, true);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (!hfc->cenDsrcTollingZone || !hfc->cenDsrcTollingZone->cenDsrcTollingZoneID) {
        databaseLockCAM_.unlock();
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

    databaseLockCAM_.unlock();
    return 0;
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
        return ERR_LOW_FREQ_CONTAINER_TYPE;
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
        return ERR_LOW_FREQ_CONTAINER_TYPE;
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

int getCAMRSUContainerHighFrequencyProtectedCommunicationZone(int stationID, int index, int *protectedZoneType, 
    int *expiryTime, int *protectedZoneLatitude, int *protectedZoneLongitude, int *protectedZoneRadius, int *protectedZoneID)
{
    databaseLockCAM_.lock();
    CAM_t* cam = getCAM(stationID);
    if (!cam) {
        databaseLockCAM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (cam->cam.camParameters.highFrequencyContainer.present != 
        HighFrequencyContainer_PR_rsuContainerHighFrequency) 
    {
        databaseLockCAM_.unlock();
        return ERR_HIGH_FREQ_CONTAINER_TYPE;
    }

    ProtectedCommunicationZonesRSU *zonesRSU = cam->cam.camParameters.highFrequencyContainer.choice
        .rsuContainerHighFrequency.protectedCommunicationZonesRSU;
    if (!zonesRSU) {
        databaseLockCAM_.unlock();
        return ERR_NULL;
    }
    
    if (index >= zonesRSU->list.count) {
        databaseLockCAM_.unlock();
        return ERR_INDEX_OUT_OF_RANGE;
    }

    ProtectedCommunicationZone_t *zone = zonesRSU->list.array[index];
    if (protectedZoneType) { *protectedZoneType = zone->protectedZoneType; }
    if (expiryTime) { getTimestamp(zone->expiryTime, expiryTime); }
    if (zone->protectedZoneRadius && protectedZoneRadius) { *protectedZoneRadius = *zone->protectedZoneRadius; }
    if (zone->protectedZoneID && protectedZoneID) { *protectedZoneID = *zone->protectedZoneID; }
    if (protectedZoneLatitude) { *protectedZoneLatitude = zone->protectedZoneLatitude; }
    if (protectedZoneLongitude) { *protectedZoneLongitude = zone->protectedZoneLongitude; }    
    
    databaseLockCAM_.unlock();
    return 0;
}

int getCAMPublicTransportContainer(int stationID, int *embarkationStatus, int *ptActivationType, uint8_t *ptActivationData, int ptActivationDataSize)
{
    databaseLockCAM_.lock();
    PublicTransportContainer *c;
    int err = getSpecialVehicleContainer(stationID, &c);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (embarkationStatus) { 
        *embarkationStatus = c->embarkationStatus; 
    }
    if (ptActivationType && c->ptActivation) { 
        *ptActivationType = c->ptActivation->ptActivationType; 
    }
    getOctetString(&c->ptActivation->ptActivationData, ptActivationData, ptActivationDataSize);

    databaseLockCAM_.unlock();
    return 0;
}

int getCAMSpecialTransportContainer(int stationID, uint8_t *specialTransportType, int specialTransportTypeSize, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize)
{
    databaseLockCAM_.lock();
    SpecialTransportContainer *c;
    int err = getSpecialVehicleContainer(stationID, &c);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    getBitString(&c->specialTransportType, specialTransportType, specialTransportTypeSize);
    getBitString(&c->lightBarSirenInUse, lightBarSirenInUse, lightBarSirenInUseSize);

    databaseLockCAM_.unlock();
    return 0;
}

int getCAMDangerousGoodsContainer(int stationID, int *dangerousGoodsBasic)
{
    databaseLockCAM_.lock();
    DangerousGoodsContainer *c;
    int err = getSpecialVehicleContainer(stationID, &c);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (dangerousGoodsBasic) {
        *dangerousGoodsBasic = c->dangerousGoodsBasic;
    }

    databaseLockCAM_.unlock();
    return 0;
}

int getCAMRoadWorksContainerBasic(int stationID, int *roadworksSubCauseCode, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize, 
    int *closedLaneInnerhardShoulderStatus, int *closedLaneOuterhardShoulderStatus, uint8_t *closedLaneDrivingLaneStatus, int closedLaneDrivingLaneStatusSize)
{
    databaseLockCAM_.lock();
    RoadWorksContainerBasic *c;
    int err = getSpecialVehicleContainer(stationID, &c);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    if (roadworksSubCauseCode && c->roadworksSubCauseCode) {
        *roadworksSubCauseCode = *c->roadworksSubCauseCode;
    }
    getBitString(&c->lightBarSirenInUse, lightBarSirenInUse, lightBarSirenInUseSize);
    if (closedLaneOuterhardShoulderStatus && c->closedLanes && c->closedLanes->outerhardShoulderStatus) {
        *closedLaneOuterhardShoulderStatus = *c->closedLanes->outerhardShoulderStatus;
    }
    if (closedLaneInnerhardShoulderStatus && c->closedLanes && c->closedLanes->innerhardShoulderStatus) {
        *closedLaneInnerhardShoulderStatus = *c->closedLanes->innerhardShoulderStatus;
    }
    if (closedLaneDrivingLaneStatus && c->closedLanes && c->closedLanes->drivingLaneStatus) {
        getBitString(c->closedLanes->drivingLaneStatus, closedLaneDrivingLaneStatus, closedLaneDrivingLaneStatusSize);
    }

    databaseLockCAM_.unlock();
    return 0;
}

int getCAMRescueContainer(int stationID, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize)
{
    databaseLockCAM_.lock();
    RescueContainer *c;
    int err = getSpecialVehicleContainer(stationID, &c);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    getBitString(&c->lightBarSirenInUse, lightBarSirenInUse, lightBarSirenInUseSize);

    databaseLockCAM_.unlock();
    return 0;
}

int getCAMEmergencyContainer(int stationID, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize, int *causeCode, 
    int *subCauseCode, uint8_t *emergencyPriority, int emergencyPrioritySize)
{
    databaseLockCAM_.lock();
    EmergencyContainer *c;
    int err = getSpecialVehicleContainer(stationID, &c);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    getBitString(&c->lightBarSirenInUse, lightBarSirenInUse, lightBarSirenInUseSize);
    if (causeCode && c->incidentIndication) {
        *causeCode = c->incidentIndication->causeCode;
    }
    if (subCauseCode && c->incidentIndication) {
        *subCauseCode = c->incidentIndication->subCauseCode;
    } 
    getBitString(c->emergencyPriority, emergencyPriority, emergencyPrioritySize);

    databaseLockCAM_.unlock();
    return 0;
}

int getCAMSafetyCarContainer(int stationID, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize, int *causeCode, int *subCauseCode, 
    int *trafficRule, int *speedLimit)
{
    databaseLockCAM_.lock();
    SafetyCarContainer *c;
    int err = getSpecialVehicleContainer(stationID, &c);
    if (err) {
        databaseLockCAM_.unlock();
        return err;
    }

    getBitString(&c->lightBarSirenInUse, lightBarSirenInUse, lightBarSirenInUseSize);
    if (causeCode && c->incidentIndication) {
        *causeCode = c->incidentIndication->causeCode;
    }
    if (subCauseCode && c->incidentIndication) {
        *subCauseCode = c->incidentIndication->subCauseCode;
    }
    if (trafficRule && c->trafficRule) {
        *trafficRule = *c->trafficRule;
    }
    if (speedLimit && c->speedLimit) {
        *speedLimit = *c->speedLimit;
    }

    databaseLockCAM_.unlock();
    return 0;
}
#pragma endregion


#pragma region De-/En-coding
int decodeCAM(int *stationID, uint8_t* buffer, int bufferSize)
{
    databaseLockCAM_.lock();

    CAM_t* cam = nullptr;
    asn_dec_rval_t retVal;
    asn_codec_ctx_t opt_codec_ctx{};
    opt_codec_ctx.max_stack_size = 0;
    retVal = xer_decode(&opt_codec_ctx, &asn_DEF_CAM, (void**)&cam, buffer, bufferSize);

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

int encodeCAM(int stationID, uint8_t* buffer, int bufferSize, int *actualBufferSize)
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
        std::stringstream errMsgStream;
        std::string xmlTagHighFrequencyContainerString = "HighFrequencyContainer";
        std::string xmlTag(retVal.failed_type->xml_tag);
        if (xmlTag.compare(xmlTagHighFrequencyContainerString) == 0) {
            errMsgStream
                << "[ERROR] No HighFrequencyContainer specified for CAM message from station '" << stationID << "'. " 
                << "Add a BasicVehicleContainerHighFrequency or a RSUContainerHighFrequency block to this message." << std::endl;
        } else {
            errMsgStream 
                << "[ERROR] CAM Encoding failed (Code=" << retVal.failed_type->name << "). " 
                << "This is probably due to an invalid value of property '" << retVal.failed_type->xml_tag
                << "' in the message of Station '" << stationID << "'." << std::endl;
        }
        setLastErrMsg(errMsgStream.str().c_str(), errMsgStream.str().size());
        std::cout << errMsgStream.str();
        return ERR_ENCODE;
    }

    size_t required_buffer_size = vectorBuffer->size();
    if (bufferSize < required_buffer_size)
    {
        delete vectorBuffer;
        return ERR_BUFFER_OVERFLOW;
    }

    ((char*)buffer)[required_buffer_size + 1] = '\0';
    size_t copiedBytes = vectorBuffer->copy(buffer, required_buffer_size);
    if (actualBufferSize) {
        *actualBufferSize = copiedBytes;
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

int setCAMIDsForTransmission(int *stationIDs_send, int numberStationIDs)
{
    return CAMTransmitter::getInstance().setIDsToTransmit(stationIDs_send, numberStationIDs);
}

#pragma endregion

#ifdef __cplusplus
};
#endif