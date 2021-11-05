#include <CAM.h>
#include <map>
#include <mutex>
#include <c2xcam.h>

static std::map<int, CAM_t*> database_;
static int databaseId_ = 0;
static std::mutex databaseLock_;

#ifdef __cplusplus
namespace c2x {
#endif

#define GET_CAM(id) auto it__ = database_.find(id); \
    if (it__ == database_.end()) { \
        databaseLock_.unlock();  \
        return ERR_MSG_NOT_FOUND; } \
    CAM_t *cam = it__->second; 

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

int createCAM(int heighFrequencyContainerType) {
    CAM_t* cam = new CAM_t();
    if (!cam) {
        return ERR_ALLOC_FAILED;
    }
    switch (heighFrequencyContainerType)
    {
    case HIGH_FREQ_CONTAINER_TYPE_NONE:
        cam->cam.camParameters.highFrequencyContainer.present
            = HighFrequencyContainer_PR::HighFrequencyContainer_PR_NOTHING;
        break;
    case HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE:
        cam->cam.camParameters.highFrequencyContainer.present
            = HighFrequencyContainer_PR::
                HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
        break;
    case HIGH_FREQ_CONTAINER_TYPE_RSU:
        cam->cam.camParameters.highFrequencyContainer.present
            = HighFrequencyContainer_PR::
                HighFrequencyContainer_PR_rsuContainerHighFrequency;
        break;
    default:
        break;
    }

    databaseLock_.lock();
    int id = ++databaseId_;
    database_.insert(std::pair<int, CAM_t*>(id, cam));
    databaseLock_.unlock();

    return id;
}

int deleteCAM(int id) {
    databaseLock_.lock();
    auto it = database_.find(id);
    if (it == database_.end()) {
        return ERR_MSG_NOT_FOUND;
    }
    ASN_STRUCT_FREE(asn_DEF_CAM, it->second);
    database_.erase(id);

    databaseLock_.unlock();
}

#pragma region Setter

int setCAMHeader(int id, int protocolVersion, int messageID, unsigned int stationID) 
{
    databaseLock_.lock();
    GET_CAM(id);

    cam->header.protocolVersion = protocolVersion;
    cam->header.messageID = messageID;
    cam->header.stationID = stationID;

    databaseLock_.unlock();
    return 0;
}

int setCAMGenerationDeltaTime(int id, int generationDeltaTime) {
    databaseLock_.lock();
    GET_CAM(id);

    cam->cam.generationDeltaTime = generationDeltaTime;

    databaseLock_.unlock();
}

int setCAMBasicContainer(int id, int stationType, Position position) {
    databaseLock_.lock();
    GET_CAM(id);

    cam->cam.camParameters.basicContainer.stationType = stationType;
    ReferencePosition *pos = 
        &cam->cam.camParameters.basicContainer.referencePosition;
    pos->latitude = position.latitude;
    pos->longitude = position.longitude;
    pos->positionConfidenceEllipse.semiMajorConfidence = 
        position.confidenceMajor;
    pos->positionConfidenceEllipse.semiMinorConfidence = 
        position.confidenceMinor;
    pos->positionConfidenceEllipse.semiMajorOrientation = 
        position.confidenceMajorOrientation;
    pos->altitude.altitudeValue = position.altitudeValue;
    pos->altitude.altitudeConfidence = position.altitudeConfidence;

    databaseLock_.unlock();
}

int setCAMBasicVehicleContainerHighFrequency(int id,
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
    databaseLock_.lock();
    GET_CAM(id);

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

    databaseLock_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencyAccelerationControl(int id, uint8_t *buffer, int size)
{
    databaseLock_.lock();
    GET_CAM(id);

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

    databaseLock_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencyLanePosition(int id, int lanePosition)
{
    databaseLock_.lock();
    GET_CAM(id);

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

    databaseLock_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(int id, int steeringWheelAngleValue, 
    int steeringWheelAngleConfidence)
{
    databaseLock_.lock();
    GET_CAM(id);

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

    databaseLock_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencyLateralAcceleration(int id, int lateralAccelerationValue, 
    int lateralAccelerationConfidence)
{
    databaseLock_.lock();
    GET_CAM(id);

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

    databaseLock_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(int id, int verticalAccelerationValue, 
    int verticalAccelerationConfidence)
{
    databaseLock_.lock();
    GET_CAM(id);

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

    databaseLock_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencyPerformaceClass(int id, int performanceClass)
{
    databaseLock_.lock();
    GET_CAM(id);

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

    databaseLock_.unlock();
}

int setCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(int id, int protectedZoneLatitude, 
    int protectedZoneLongitude, int cenDsrcTollingZoneID)
{
    databaseLock_.lock();
    GET_CAM(id);
    
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

    databaseLock_.unlock();
}


int setCAMBasicVehicleContainerLowFrequency(int id, int vehicleRole, uint8_t *exteriorLights, int exteriorLightsSize)
{
    databaseLock_.lock();
    GET_CAM(id);
    
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

    databaseLock_.unlock();
}

int addCAMBasicVehicleContainerLowFrequencyPathPoint(int id, DeltaPathPoint pathPoint)
{
    databaseLock_.lock();
    GET_CAM(id);
    
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
    *pp->pathDeltaTime = pathPoint.deltaTime;
    pp->pathPosition.deltaLatitude = pathPoint.deltaLatitude;
    pp->pathPosition.deltaLongitude = pathPoint.deltaLongitude;
    pp->pathPosition.deltaAltitude = pathPoint.deltaAltitude;
    lfc->pathHistory.list.free = freePathPoint;
    asn_sequence_add(&lfc->pathHistory.list, pp);

    databaseLock_.unlock();
}

#pragma endregion

#pragma region Getter
void getCAMHeader(CAM_t* cam, int *protocolVersion, int *messageID, unsigned int *stationID)
{
    if (protocolVersion) {
        *protocolVersion = cam->header.protocolVersion;        
    }
    if (messageID) {
        *messageID = cam->header.messageID;
    }
    if (stationID) {
        *stationID = cam->header.stationID;
    }
}

int getCAMHeader(int id, int *protocolVersion, int *messageID, unsigned int *stationID) 
{
    databaseLock_.lock();
    GET_CAM(id);

    getCAMHeader(cam, protocolVersion, messageID, stationID);

    databaseLock_.unlock();
    return 0;
}

int getCAMHeader_recv(int stationId, int *protocolVersion, int *messageID, unsigned int *stationID) 
{
    // TODO
}

void getCAMGenerationDeltaTime(CAM_t* cam, int *generationDeltaTime) 
{
    if (generationDeltaTime) {
        *generationDeltaTime = cam->cam.generationDeltaTime;
    }
}

int getCAMGenerationDeltaTime(int id, int *generationDeltaTime) 
{
    databaseLock_.lock();
    GET_CAM(id);

    getCAMGenerationDeltaTime(cam, generationDeltaTime);

    databaseLock_.unlock();
    return 0;
}

int getCAMGenerationDeltaTime_recv(int stationId, int *generationDeltaTime) 
{
    // TODO
}

void getCAMBasicContainer(CAM_t* cam, int *stationType, Position *position) 
{
    if (stationType) {
        *stationType = cam->cam.camParameters.basicContainer.stationType;
    }
    if (position) {
        ReferencePosition* pos = 
            &cam->cam.camParameters.basicContainer.referencePosition;
        position->latitude = pos->latitude;
        position->longitude = pos->longitude;
        position->confidenceMajor = 
            pos->positionConfidenceEllipse.semiMajorConfidence; 
        position->confidenceMinor = 
            pos->positionConfidenceEllipse.semiMinorConfidence;
        position->confidenceMajorOrientation = 
            pos->positionConfidenceEllipse.semiMajorOrientation;
        position->altitudeValue = pos->altitude.altitudeValue;
        position->altitudeConfidence = pos->altitude.altitudeConfidence;
    }
}

int getCAMBasicContainer(int id, int *stationType, Position *position) 
{
    databaseLock_.lock();
    GET_CAM(id);

    getCAMBasicContainer(cam, stationType, position);

    databaseLock_.unlock();
    return 0;
}

int getCAMBasicContainer_recv(int stationId, int *stationType, Position *position) 
{
    // TODO
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

int getCAMBasicVehicleContainerHighFrequency(int id, int *headingValue, int *headingConfidence, int *speedValue, 
    int *speedConfidence, int *driveDirection,int *vehicleLengthValue, int *vehicleLengthConfidenceIndication,
    int *vehicleWidth, int *longitudinalAccelerationValue, int *longitudinalAccelerationConfidence,
    int *curvatureValue, int *curvatureConfidence, int *curvatureCalculationMode, int *yawRateValue, 
    int *yawRateConfidence)
{
    databaseLock_.lock();
    GET_CAM(id);

    int ret = getCAMBasicVehicleContainerHighFrequency(cam, headingValue, headingConfidence, speedValue, 
        speedConfidence, driveDirection, vehicleLengthValue, vehicleLengthConfidenceIndication, vehicleWidth, 
        longitudinalAccelerationValue, longitudinalAccelerationConfidence, curvatureValue, curvatureConfidence, 
        curvatureCalculationMode, yawRateValue, yawRateConfidence);

    databaseLock_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequency_recv(int stationId, int *headingValue, int *headingConfidence, 
    int *speedValue, int *speedConfidence, int *driveDirection,int *vehicleLengthValue, 
    int *vehicleLengthConfidenceIndication, int *vehicleWidth, int *longitudinalAccelerationValue, 
    int *longitudinalAccelerationConfidence, int *curvatureValue, int *curvatureConfidence, 
    int *curvatureCalculationMode, int *yawRateValue, int *yawRateConfidence)
{
    // TODO
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
        cpySize = std::min(size, hfc->accelerationControl->size);
        memcpy(buffer, hfc->accelerationControl, cpySize);
    }
    if (actualSize) {
        *actualSize = cpySize;
    }
}

int getCAMBasicVehicleContainerHighFrequencyAccelerationControl(int id, uint8_t *buffer, int size, int* actualSize)
{
    databaseLock_.lock();
    GET_CAM(id);

    int ret = getCAMBasicVehicleContainerHighFrequencyAccelerationControl(cam, buffer, size, actualSize);

    databaseLock_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencyAccelerationControl_recv(int id, uint8_t *buffer, int size, int* actualSize)
{
    // TODO
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

int getCAMBasicVehicleContainerHighFrequencyLanePosition(int id, int *lanePosition)
{
    databaseLock_.lock();
    GET_CAM(id);

    int ret = getCAMBasicVehicleContainerHighFrequencyLanePosition(cam, lanePosition);

    databaseLock_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencyLanePosition_recv(int id, int *lanePosition)
{
    // TODO
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

int getCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(int id, int *steeringWheelAngleValue, 
    int *steeringWheelAngleConfidence)
{
    databaseLock_.lock();
    GET_CAM(id);

    int ret = getCAMBasicVehicleContainerHighFrequencySteeringWheelAngle(cam,
        steeringWheelAngleValue, steeringWheelAngleConfidence);

    databaseLock_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencySteeringWheelAngle_recv(
    int id, int *steeringWheelAngleValue, int *steeringWheelAngleConfidence)
{
    // TODO
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

int getCAMBasicVehicleContainerHighFrequencyLateralAcceleration(int id,
    int *lateralAccelerationValue, int *lateralAccelerationConfidence)
{
    databaseLock_.lock();
    GET_CAM(id);

    int ret = getCAMBasicVehicleContainerHighFrequencyLateralAcceleration(cam,
        lateralAccelerationValue, lateralAccelerationConfidence);

    databaseLock_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencyLateralAcceleration_recv(int id,
    int *lateralAccelerationValue, int *lateralAccelerationConfidence)
{
    // TODO
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

int getCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(int id,
    int *verticalAccelerationValue, int *verticalAccelerationConfidence)
{
    databaseLock_.lock();
    GET_CAM(id);

    int ret = getCAMBasicVehicleContainerHighFrequencyVerticalAcceleration(cam,
        verticalAccelerationValue, verticalAccelerationConfidence);

    databaseLock_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencyVerticalAcceleration_recv(int id,
    int *verticalAccelerationValue, int *verticalAccelerationConfidence)
{
    // TODO
}

int getCAMBasicVehicleContainerHighFrequencyPerformaceClass(CAM_t *cam, int *performanceClass)
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

int getCAMBasicVehicleContainerHighFrequencyPerformaceClass(int id, int *performanceClass)
{
    databaseLock_.lock();
    GET_CAM(id);

    int ret = getCAMBasicVehicleContainerHighFrequencyPerformaceClass(cam, performanceClass);

    databaseLock_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencyPerformaceClass_recv(int id, int *performanceClass)
{
    // TODO
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

int getCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(int id,
    int *protectedZoneLatitude, int *protectedZoneLongitude, int *cenDsrcTollingZoneID)
{
    databaseLock_.lock();
    GET_CAM(id);

    int ret = getCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone(cam,
        protectedZoneLatitude, protectedZoneLongitude, cenDsrcTollingZoneID);

    databaseLock_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerHighFrequencyCenDsrcTollingZone_recv(int id,
    int *protectedZoneLatitude, int *protectedZoneLongitude, int *cenDsrcTollingZoneID)
{
    // TODO
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
        cpySize = std::min(exteriorLightsSize, lfc->exteriorLights.size);
        memcpy(exteriorLights, lfc->exteriorLights.buf, cpySize);
    }
    if (actualExteriorLightsSize) {
        *actualExteriorLightsSize = lfc->exteriorLights.size;
    }
}

int getCAMBasicVehicleContainerLowFrequency(int id, int *vehicleRole, uint8_t *exteriorLights, 
    int exteriorLightsSize, int *actualExteriorLightsSize)
{
    databaseLock_.lock();
    GET_CAM(id);

    int ret = getCAMBasicVehicleContainerLowFrequency(cam, vehicleRole, exteriorLights, exteriorLightsSize, 
        actualExteriorLightsSize);

    databaseLock_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerLowFrequency_recv(int stationId, int *vehicleRole, uint8_t *exteriorLights, 
    int exteriorLightsSize, int *actualExteriorLightsSize)
{
    // TODO
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
        int cpySize = std::min((int)(pathHistorySize / 4), len);
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

int getCAMBasicVehicleContainerLowFrequencyPathHistory(int id, int* pathHistory, 
    int pathHistorySize, int *actualPathHistorySize)
{
    databaseLock_.lock();
    GET_CAM(id);

    int ret = getCAMBasicVehicleContainerLowFrequencyPathHistory(cam, pathHistory, pathHistorySize, 
        actualPathHistorySize);

    databaseLock_.unlock();
    return ret;
}

int getCAMBasicVehicleContainerLowFrequencyPathHistory_recv(int stationId, DeltaPathPoint* pathHistory, 
    int pathHistorySize, int *actualPathHistorySize)
{
    // TODO
}


#pragma endregion

#ifdef __cplusplus
};
#endif