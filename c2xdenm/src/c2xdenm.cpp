#include <c2xdenm.h>
#include <DENMReceiver.hpp>
#include <DENMTransmitter.hpp>
#include <iostream>
#include <map>
#include <DENM.h>
#include <VectorBuffer.hpp>
#include <sstream>

#ifdef __cplusplus
namespace c2x {
#endif

typedef struct ActionId {
    int stationID_ = 0;
    int sequenceNumber_ = 0;
    ActionId(ActionID_t &aID) : stationID_(aID.originatingStationID), sequenceNumber_(aID.sequenceNumber) {}
    friend bool operator<(const ActionId& a1, const ActionId& a2) {
        return (((uint64_t)a1.stationID_ << (uint64_t)32) + (uint64_t)a1.sequenceNumber_) < 
            (((uint64_t)a2.stationID_ << (uint64_t)32) + (uint64_t)a2.sequenceNumber_);
    }
} ActionId;

static std::map<ActionId, DENM_t*> databaseDENM_;
static std::mutex databaseLockDENM_;
static c2x::Buffer* writeCallbackBufferDENM_;
static std::mutex writeCallbackLockDENM_;

std::map<ActionId, DENM_t*>::iterator getDENMiterator(int stationID, int sequenceNumber) {
    for (auto it = databaseDENM_.begin(); it != databaseDENM_.end(); it++) {
        if (it->first.stationID_ == stationID && it->first.sequenceNumber_ == sequenceNumber) {
            return it;
        }
    }
    return databaseDENM_.end();
}

DENM_t* getDENM(int stationID, int sequenceNumber) {
    auto it = getDENMiterator(stationID, sequenceNumber);
    if (it != databaseDENM_.end()) 
    {
        return it->second;
    }
    return nullptr;
}

std::map<ActionId, DENM_t*>::iterator getDENMiterator(ActionID_t aID) {
    return getDENMiterator(aID.originatingStationID, aID.sequenceNumber);
}

std::map<ActionId, DENM_t*>::iterator getDENMiterator(ActionId aID) {
    return getDENMiterator(aID.stationID_, aID.sequenceNumber_);
}

DENM* getDENM(ActionID_t aID) {
    return getDENM(aID.originatingStationID, aID.sequenceNumber);
}

DENM* getDENM(ActionId aID) {
    return getDENM(aID.stationID_, aID.sequenceNumber_);
}

void freeEventPoint(EventPoint *point)
{
    if (!point) {
        return;
    }
    delete point->eventDeltaTime;
}

void freePathPoint(PathPoint *pathPoint) {
    if (!pathPoint) {
        return;
    }
    delete pathPoint->pathDeltaTime;
    delete pathPoint;
}

void freePathHistory(PathHistory *pathHistory)
{
    if (!pathHistory)
    {
        return;
    }
    while (pathHistory->list.count > 0) 
    {
        asn_sequence_del(&pathHistory->list, 0, 1);
    }
    delete pathHistory;
}

void freePosPillar(PosPillar_t *pillar) {
    if (!pillar) {
        return;
    }
    delete pillar;
}

void freeStationType(StationType_t *stationType) {
    if (!stationType) {
        return;
    }
    delete stationType;
}

void freeReferencePosition(ReferencePosition *refPos) {
    if (!refPos) {
        return;
    }
    delete refPos;
}

void freeActionID(ActionID *actionID) {
    if (!actionID) {
        return;
    }
    delete actionID;
}

void setBitString(BIT_STRING_t *bitString, uint8_t *data, int dataSize) {
    if (!bitString || !data) {
        return;
    }

    if (!bitString->buf) {
        bitString->buf = (uint8_t*)malloc(dataSize);
        if (!bitString->buf) {
            return;
        }
    }
    else {
        uint8_t* tmp_buf = (uint8_t*)realloc(bitString->buf, dataSize);
        if (!tmp_buf) {
            delete bitString->buf;
            return;
        } else {
           bitString->buf = tmp_buf;
        }
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
        if (!octetString->buf) {
            return;
        }
    }
    else {
        uint8_t* tmp_buf = (uint8_t*)realloc(octetString->buf, dataSize);
        if (!tmp_buf) {
            delete octetString->buf;
            return;
        }
        else {
            octetString->buf = tmp_buf;
        }
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
        if (!timestamp->buf) {
            return;
        }
    }
    else if (timestamp->size != 4) {
        uint8_t* tmp_buf = (uint8_t*)realloc(timestamp->buf, sizeof(int));
        if (!tmp_buf) {
            delete timestamp->buf;
            return;
        }
        else {
            timestamp->buf = tmp_buf;
        }
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

int startDENMReceiver(int port)
{
    try
    {
        DENMReceiver::getInstance().start(port);
    }
    catch (const std::exception& ex)
    {
        std::cout << "" << ex.what() << std::endl;
        return ERR_RECEIVER_START;
    }
    return 0;
}

int stopDENMReceiver()
{
    DENMReceiver::getInstance().stop();
    return 0;
}

int startDENMTransmitter(int port)
{
    try
    {
        DENMTransmitter::getInstance().start(port);
    }
    catch (const std::exception& ex)
    {
        std::cout << "" << ex.what() << std::endl;
        return ERR_TRANSMITTER_START;
    }
    return 0;
}

int stopDENMTransmitter()
{
    DENMTransmitter::getInstance().stop();
    return 0;
}

int setDENMTransmissionFrequency(double f)
{
    unsigned int int_ms = (unsigned int)(1000 / f);
    DENMTransmitter::getInstance().setInterval(int_ms);
    return 0;
}

int createDENM(int stationID, int sequenceNumber)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (denm) {
        databaseLockDENM_.unlock();
        return ERR_DENM_ALREADY_EXISTS;
    }

    denm = new DENM_t();
    denm->header.stationID = stationID;
    denm->denm.management.actionID.originatingStationID = stationID;
    denm->denm.management.actionID.sequenceNumber = sequenceNumber;

    denm->denm.management.detectionTime.buf = new uint8_t[0];
    denm->denm.management.detectionTime.size = 0;
    denm->denm.management.referenceTime.buf = new uint8_t[0];
    denm->denm.management.referenceTime.size = 0;

    denm->denm.management.termination = new Termination_t();
    denm->denm.management.relevanceDistance = new RelevanceDistance_t();
    denm->denm.management.relevanceTrafficDirection = new RelevanceTrafficDirection_t();
    denm->denm.management.validityDuration = new ValidityDuration_t();
    denm->denm.management.transmissionInterval = new TransmissionInterval_t();

    denm->denm.situation = nullptr;
    denm->denm.location = nullptr;
    denm->denm.alacarte = nullptr;

    ActionId aID(denm->denm.management.actionID);
    databaseDENM_.insert(std::pair<ActionId, DENM_t*>(aID, denm));
    databaseLockDENM_.unlock();

    return 0;
}

int deleteDENM(int stationID, int sequenceNumber)
{
    databaseLockDENM_.lock();
    auto it = getDENMiterator(stationID, sequenceNumber);
    if (it == databaseDENM_.end()) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    ActionId aID = it->first;
    ASN_STRUCT_FREE(asn_DEF_DENM, it->second);
    databaseDENM_.erase(aID);
    databaseLockDENM_.unlock();
    return 0;
}

int setDENMHeader(int stationID, int sequenceNumber, int protocolVersion, int messageID)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (denm == nullptr)
    {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    denm->header.protocolVersion = protocolVersion;
    denm->header.messageID = messageID;

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMManagementContainer(int stationID, int sequenceNumber, int originatingStationID, int detectionTime, int referenceTime, int termination, 
    int relevanceDistance, int relevanceTrafficDirection, int validityDuration, int transmissionInterval, int stationType)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (denm == nullptr)
    {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    setTimestamp(&denm->denm.management.detectionTime, detectionTime);
    setTimestamp(&denm->denm.management.referenceTime, referenceTime);    

    if (!denm->denm.management.termination) {
        denm->denm.management.termination = new Termination_t();
    }
    *denm->denm.management.termination = termination;
    
    if (!denm->denm.management.relevanceDistance) {
        denm->denm.management.relevanceDistance = new RelevanceDistance_t();
    }
    *denm->denm.management.relevanceDistance = relevanceDistance;

    if (!denm->denm.management.relevanceTrafficDirection) {
        denm->denm.management.relevanceTrafficDirection = new RelevanceTrafficDirection_t();
    }
    *denm->denm.management.relevanceTrafficDirection = relevanceTrafficDirection;

    if (!denm->denm.management.validityDuration) {
        denm->denm.management.validityDuration = new ValidityDuration_t();
    }
    *denm->denm.management.validityDuration = validityDuration;

    if (!denm->denm.management.transmissionInterval) {
        denm->denm.management.transmissionInterval = new TransmissionInterval_t();
    }
    *denm->denm.management.transmissionInterval = transmissionInterval;

    denm->denm.management.stationType = stationType;

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMManagementContainerEventPosition(int stationID, int sequenceNumber, int latitude, int longitude,
    int posConfEllSemiMajorConf, int posConfEllSemiMinorConf, int posConfEllSemiMajorOri, int altitudeValue, int altitudeConf)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm)
    {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    denm->denm.management.eventPosition.latitude = latitude;
    denm->denm.management.eventPosition.longitude = longitude;
    denm->denm.management.eventPosition.positionConfidenceEllipse.semiMajorConfidence = posConfEllSemiMajorConf;
    denm->denm.management.eventPosition.positionConfidenceEllipse.semiMinorConfidence = posConfEllSemiMinorConf;
    denm->denm.management.eventPosition.positionConfidenceEllipse.semiMajorOrientation = posConfEllSemiMajorOri;
    denm->denm.management.eventPosition.altitude.altitudeValue = altitudeValue;
    denm->denm.management.eventPosition.altitude.altitudeConfidence = altitudeConf;

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMSituationContainer(int stationID, int sequenceNumber, int informationQuality, int causeCode, int subCauseCode)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm)
    {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (!denm->denm.situation) 
    {
        denm->denm.situation = new SituationContainer();
    }
    SituationContainer* sc = denm->denm.situation;

    sc->informationQuality = informationQuality;
    sc->eventType.causeCode = causeCode;
    sc->eventType.subCauseCode = subCauseCode;

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMSituationContainerLinkedCause(int stationID, int sequenceNumber, int causeCode, int subCauseCode)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm)
    {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (!denm->denm.situation) 
    {
        denm->denm.situation = new SituationContainer();
    }
    SituationContainer* sc = denm->denm.situation;

    if (!sc->linkedCause) 
    {
        sc->linkedCause = new CauseCode();
    }

    sc->linkedCause->causeCode = causeCode;
    sc->linkedCause->subCauseCode = subCauseCode;

    databaseLockDENM_.unlock();
    return 0;
}

int addDENMSituationContainerEventPoint(int stationID, int sequenceNumber, int deltaLatitude, int deltaLongitude, int deltaAltitude, int deltaTime, int informationQuality)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm)
    {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (!denm->denm.situation) 
    {
        denm->denm.situation = new SituationContainer();
    }
    SituationContainer* sc = denm->denm.situation;

    if (!sc->eventHistory) 
    {
        sc->eventHistory = new EventHistory();
        sc->eventHistory->list.free = freeEventPoint;
    }

    EventPoint *point = new EventPoint();
    point->eventPosition.deltaLatitude = deltaLatitude;
    point->eventPosition.deltaLongitude = deltaLongitude;
    point->eventPosition.deltaAltitude = deltaAltitude;
    point->eventDeltaTime = new PathDeltaTime_t();
    *point->eventDeltaTime = deltaTime;
    point->informationQuality = informationQuality;
    asn_sequence_add(&sc->eventHistory->list, point);

    databaseLockDENM_.unlock();
    return 0;
}

int addDENMLocationContainerTrace(int stationID, int sequenceNumber, int* trace, int traceLength)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm)
    {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (!denm->denm.location) 
    {
        denm->denm.location = new LocationContainer();
        denm->denm.location->traces.list.free = freePathHistory;
    }
    LocationContainer* lc = denm->denm.location;

    PathHistory *newHistory = new PathHistory();
    newHistory->list.free = freePathPoint;
    int len = (int)(traceLength / 4);
    for (int i = 0; i < len; i++)
    {
        PathPoint *pathPoint = new PathPoint();
        pathPoint->pathDeltaTime = new PathDeltaTime_t();
        pathPoint->pathPosition.deltaLatitude = trace[i*4 + 0];
        pathPoint->pathPosition.deltaLongitude = trace[i*4 + 1];
        pathPoint->pathPosition.deltaAltitude = trace[i*4 + 2];
        *pathPoint->pathDeltaTime = trace[i*4 + 3];
        asn_sequence_add(&newHistory->list, pathPoint);
    }
    asn_sequence_add(&lc->traces.list, newHistory);

    databaseLockDENM_.unlock();
    return 0;
}

int clearDENMLocationContainerTraces(int stationID, int sequenceNumber)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm)
    {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (!denm->denm.location) 
    {
        databaseLockDENM_.unlock();
        return 0;
    }
    LocationContainer* lc = denm->denm.location;

    asn_sequence_empty(&lc->traces.list);

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMLocationContainerSpeed(int stationID, int sequenceNumber, int speedValue, int speedConfidence)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm)
    {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (!denm->denm.location) 
    {
        denm->denm.location = new LocationContainer();
        denm->denm.location->traces.list.free = freePathHistory;
    }
    LocationContainer* lc = denm->denm.location;

    if (!lc->eventSpeed)
    {
        lc->eventSpeed = new Speed();
    }
    lc->eventSpeed->speedValue = speedValue;
    lc->eventSpeed->speedConfidence = speedConfidence;

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMLocationContainerHeading(int stationID, int sequenceNumber, int headingValue, int headingConfidence)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm)
    {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (!denm->denm.location) 
    {
        denm->denm.location = new LocationContainer();
        denm->denm.location->traces.list.free = freePathHistory;
    }
    LocationContainer* lc = denm->denm.location;

    if (!lc->eventPositionHeading)
    {
        lc->eventPositionHeading = new Heading();
    }
    lc->eventPositionHeading->headingValue = headingValue;
    lc->eventPositionHeading->headingConfidence = headingConfidence;

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMLocationContainerRoadType(int stationID, int sequenceNumber, int roadType)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm)
    {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    if (!denm->denm.location) 
    {
        denm->denm.location = new LocationContainer();
        denm->denm.location->traces.list.free = freePathHistory;
    }
    LocationContainer* lc = denm->denm.location;

    if (!lc->roadType)
    {
        lc->roadType = new RoadType_t();
    }
    *lc->roadType = roadType;

    databaseLockDENM_.unlock();
    return 0;
}

int getAlacarteContainerInitialized(int stationID, int sequenceNumber, AlacarteContainer **ac) 
{
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm)
    {
        return ERR_MSG_NOT_FOUND;
    }

    if (!denm->denm.alacarte) {
        denm->denm.alacarte = new AlacarteContainer();
    }

    *ac = denm->denm.alacarte;
    return 0;
}

int setDENMAlacarteContainer(int stationID, int sequenceNumber, int lanePosition, int externalTemperature, int positioningSolution)
{
    databaseLockDENM_.lock();
    AlacarteContainer* ac;
    int err = getAlacarteContainerInitialized(stationID, sequenceNumber, &ac);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!ac->lanePosition) {
        ac->lanePosition = new LanePosition_t();
    }
    *ac->lanePosition = lanePosition;

    if (!ac->externalTemperature) {
        ac->externalTemperature = new Temperature_t();
    }
    *ac->externalTemperature = externalTemperature;

    if (!ac->positioningSolution) {
        ac->positioningSolution = new PositioningSolutionType_t();
    }
    *ac->positioningSolution = positioningSolution;

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMImpactReductionContainer(int stationID, int sequenceNumber, int heightLonCarrLeft, int heightLonCarrRight, int posLonCarrLeft, 
    int posLonCarrRight, int *positionOfPillars, int positionOfPillarsSize, int posCentMass, int wheelBaseVehicle, int turningRadius, int posFrontAx, 
    uint8_t *positionOfOccupants, int positionOfOccupantsSize, int vehicleMass, int requestResponseIndication)
{
    databaseLockDENM_.lock();
    AlacarteContainer* ac;
    int err = getAlacarteContainerInitialized(stationID, sequenceNumber, &ac);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!ac->impactReduction) {
        ac->impactReduction = new ImpactReductionContainer();
    }
    ac->impactReduction->heightLonCarrLeft = heightLonCarrLeft;
    ac->impactReduction->heightLonCarrRight = heightLonCarrRight;
    ac->impactReduction->posLonCarrLeft = posLonCarrLeft;
    ac->impactReduction->posLonCarrRight = posLonCarrRight;
    ac->impactReduction->posCentMass = posCentMass;
    ac->impactReduction->wheelBaseVehicle = wheelBaseVehicle;
    ac->impactReduction->turningRadius = turningRadius;
    ac->impactReduction->posFrontAx = posFrontAx;
    ac->impactReduction->vehicleMass = vehicleMass;
    ac->impactReduction->requestResponseIndication = requestResponseIndication;
    
    ac->impactReduction->positionOfPillars.list.free = freePosPillar;
    asn_sequence_empty(&ac->impactReduction->positionOfPillars.list);
    for (int i = 0; i < positionOfPillarsSize; i++) {
        PosPillar_t *pillar = new PosPillar_t();
        *pillar = positionOfPillars[i];
        asn_sequence_add(&ac->impactReduction->positionOfPillars.list, pillar);
    }

    setBitString(&ac->impactReduction->positionOfOccupants, positionOfOccupants, positionOfOccupantsSize);

    databaseLockDENM_.unlock();
    return 0;
}    

int getRoadWorksContainerExtendedInitialized(int stationID, int sequenceNumber, RoadWorksContainerExtended **rwc) 
{
    AlacarteContainer* ac;
    int err = getAlacarteContainerInitialized(stationID, sequenceNumber, &ac);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!ac->roadWorks) {
        ac->roadWorks = new RoadWorksContainerExtended();
    }
    *rwc = ac->roadWorks;
    return 0;
}

int setDENMRoadWorksContainerExtendedLightBarSiren(int stationID, int sequenceNumber, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended* rwc;
    int err = getRoadWorksContainerExtendedInitialized(stationID, sequenceNumber, &rwc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->lightBarSirenInUse) {
        rwc->lightBarSirenInUse = new LightBarSirenInUse_t();
    }
    setBitString(rwc->lightBarSirenInUse, lightBarSirenInUse, lightBarSirenInUseSize);

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMRoadWorksContainerExtendedClosedLanes(int stationID, int sequenceNumber, int innerhardShoulderStatus, int outerhardShoulderStatus, int drivingLaneStatus)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended* rwc;
    int err = getRoadWorksContainerExtendedInitialized(stationID, sequenceNumber, &rwc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->closedLanes) {
        rwc->closedLanes = new ClosedLanes();
        rwc->closedLanes->innerhardShoulderStatus = new HardShoulderStatus_t();
        rwc->closedLanes->outerhardShoulderStatus = new HardShoulderStatus_t();
        rwc->closedLanes->drivingLaneStatus = new DrivingLaneStatus_t();
    }
    *rwc->closedLanes->innerhardShoulderStatus = innerhardShoulderStatus;
    *rwc->closedLanes->outerhardShoulderStatus = outerhardShoulderStatus;
    setBitString(rwc->closedLanes->drivingLaneStatus, (uint8_t*)(&drivingLaneStatus), sizeof(int));   

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMRoadWorksContainerExtendedRestriction(int stationID, int sequenceNumber, int *stationTypes, int stationTypesSize)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended* rwc;
    int err = getRoadWorksContainerExtendedInitialized(stationID, sequenceNumber, &rwc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->restriction) {
        rwc->restriction = new RestrictedTypes_t();
        rwc->restriction->list.free = freeStationType;
    }
    asn_sequence_empty(&rwc->restriction->list);
    for (int i = 0; i < stationTypesSize; i++) {
        StationType_t *stationType = new StationType_t();
        *stationType = stationTypes[i];
        asn_sequence_add(&rwc->restriction->list, stationType);
    }

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMRoadWorksContainerExtendedSpeedLimit(int stationID, int sequenceNumber, int speedLimit)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended* rwc;
    int err = getRoadWorksContainerExtendedInitialized(stationID, sequenceNumber, &rwc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->speedLimit) {
        rwc->speedLimit = new SpeedLimit_t();
    }
    *rwc->speedLimit = speedLimit;

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMRoadWorksContainerExtendedIncidentIndication(int stationID, int sequenceNumber, int causeCode, int subCauseCode)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended* rwc;
    int err = getRoadWorksContainerExtendedInitialized(stationID, sequenceNumber, &rwc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->incidentIndication) {
        rwc->incidentIndication = new CauseCode_t();
    }
    rwc->incidentIndication->causeCode = causeCode;
    rwc->incidentIndication->subCauseCode = subCauseCode;

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMRoadWorksContainerExtendedRecommendedPath(int stationID, int sequenceNumber, int *referencePositions, int referencePositionsSize)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended* rwc;
    int err = getRoadWorksContainerExtendedInitialized(stationID, sequenceNumber, &rwc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->recommendedPath) {
        rwc->recommendedPath = new ItineraryPath_t();
        rwc->recommendedPath->list.free = freeReferencePosition;
    }
    asn_sequence_empty(&rwc->recommendedPath->list);
    int nrPositions = (int)(referencePositionsSize / 7);
    for (int i = 0; i < nrPositions; i++)
    {
        ReferencePosition *refPos = new ReferencePosition();
        refPos->latitude = referencePositions[i*7 + 0];
        refPos->longitude = referencePositions[i*7 + 1];
        refPos->positionConfidenceEllipse.semiMajorConfidence = referencePositions[i*7 + 2];
        refPos->positionConfidenceEllipse.semiMinorConfidence = referencePositions[i*7 + 3];
        refPos->positionConfidenceEllipse.semiMajorOrientation = referencePositions[i*7 + 4];
        refPos->altitude.altitudeValue = referencePositions[i*7 + 5];
        refPos->altitude.altitudeConfidence = referencePositions[i*7 + 6];
        asn_sequence_add(&rwc->recommendedPath->list, refPos);
    }    

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMRoadWorksContainerExtendedStartingPointSpeedLimit(int stationID, int sequenceNumber, int deltaLatitude, int deltaLongitude, int deltaAltitude)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended* rwc;
    int err = getRoadWorksContainerExtendedInitialized(stationID, sequenceNumber, &rwc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->startingPointSpeedLimit) {
        rwc->startingPointSpeedLimit = new DeltaReferencePosition();
    }
    rwc->startingPointSpeedLimit->deltaLatitude = deltaLatitude;
    rwc->startingPointSpeedLimit->deltaLongitude = deltaLongitude;
    rwc->startingPointSpeedLimit->deltaAltitude = deltaAltitude;

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMRoadWorksContainerExtendedTrafficFlowRule(int stationID, int sequenceNumber, int trafficFlowRule)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended* rwc;
    int err = getRoadWorksContainerExtendedInitialized(stationID, sequenceNumber, &rwc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->trafficFlowRule) {
        rwc->trafficFlowRule = new TrafficRule_t();
    }
    *rwc->trafficFlowRule = trafficFlowRule;

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMRoadWorksContainerExtendedReferenceDenms(int stationID, int sequenceNumber, int *actionIDs, int actionIDsSize)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended* rwc;
    int err = getRoadWorksContainerExtendedInitialized(stationID, sequenceNumber, &rwc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->referenceDenms) {
        rwc->referenceDenms = new ReferenceDenms();
        rwc->referenceDenms->list.free = freeActionID;
    }
    asn_sequence_empty(&rwc->referenceDenms->list);
    int nrActionIDs = (int)(actionIDsSize / 2);
    for (int i = 0; i < nrActionIDs; i++) {
        ActionID *aID = new ActionID();
        aID->originatingStationID = actionIDs[i*2 + 0];
        aID->sequenceNumber = actionIDs[i*2 + 1];
        asn_sequence_add(&rwc->referenceDenms->list, aID);
    }
    
    databaseLockDENM_.unlock();
    return 0;
}

int getStationaryVehicleContainerInitialized(int stationID, int sequenceNumber, StationaryVehicleContainer **swc) 
{
    AlacarteContainer* ac;
    int err = getAlacarteContainerInitialized(stationID, sequenceNumber, &ac);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!ac->stationaryVehicle) {
        ac->stationaryVehicle = new StationaryVehicleContainer();
    }
    *swc = ac->stationaryVehicle;
    return 0;
}

int setDENMStationaryVehicleContainerStationarySince(int stationID, int sequenceNumber, int stationarySince)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer* swc;
    int err = getStationaryVehicleContainerInitialized(stationID, sequenceNumber, &swc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->stationarySince) {
        swc->stationarySince = new StationarySince_t();
    }
    *swc->stationarySince = stationarySince;
    
    databaseLockDENM_.unlock();
    return 0;
}

int setDENMStationaryVehicleContainerStationaryCause(int stationID, int sequenceNumber, int causeCode, int subCauseCode)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer* swc;
    int err = getStationaryVehicleContainerInitialized(stationID, sequenceNumber, &swc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->stationaryCause) {
        swc->stationaryCause = new CauseCode();
    }
    swc->stationaryCause->causeCode = causeCode;
    swc->stationaryCause->subCauseCode = subCauseCode;
    
    databaseLockDENM_.unlock();
    return 0;
}

int setDENMStationaryVehicleContainerCarryingDangerousGoods(int stationID, int sequenceNumber, int dangerousGoodsType, 
    int unNumber, int elevatedTemperature, int tunnelsRestricted, int limitedQuantity)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer* swc;
    int err = getStationaryVehicleContainerInitialized(stationID, sequenceNumber, &swc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->carryingDangerousGoods) {
        swc->carryingDangerousGoods = new DangerousGoodsExtended();
    }
    swc->carryingDangerousGoods->dangerousGoodsType = dangerousGoodsType;
    swc->carryingDangerousGoods->unNumber = unNumber;
    swc->carryingDangerousGoods->elevatedTemperature = elevatedTemperature;
    swc->carryingDangerousGoods->tunnelsRestricted = tunnelsRestricted;
    swc->carryingDangerousGoods->limitedQuantity = limitedQuantity;
    
    databaseLockDENM_.unlock();
    return 0;
}

int setDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode(int stationID, int sequenceNumber, 
    uint8_t *emergencyActionCode, int emergencyActionCodeSize)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer* swc;
    int err = getStationaryVehicleContainerInitialized(stationID, sequenceNumber, &swc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->carryingDangerousGoods) {
        swc->carryingDangerousGoods = new DangerousGoodsExtended();
    }
    if (!swc->carryingDangerousGoods->emergencyActionCode) {
        swc->carryingDangerousGoods->emergencyActionCode = new IA5String_t();
    }
    setOctetString(swc->carryingDangerousGoods->emergencyActionCode, emergencyActionCode, emergencyActionCodeSize);

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMStationaryVehicleContainerCarryingDangerousGoodsPhoneNumber(int stationID, int sequenceNumber, uint8_t *phoneNumber, int phoneNumberSize)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer* swc;
    int err = getStationaryVehicleContainerInitialized(stationID, sequenceNumber, &swc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->carryingDangerousGoods) {
        swc->carryingDangerousGoods = new DangerousGoodsExtended();
    }
    if (!swc->carryingDangerousGoods->phoneNumber) {
        swc->carryingDangerousGoods->phoneNumber = new PhoneNumber_t();
    }
    setOctetString(swc->carryingDangerousGoods->phoneNumber, phoneNumber, phoneNumberSize);

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMStationaryVehicleContainerCarryingDangerousGoodsCompanyName(int stationID, int sequenceNumber, uint8_t *companyName, int companyNameSize)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer* swc;
    int err = getStationaryVehicleContainerInitialized(stationID, sequenceNumber, &swc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->carryingDangerousGoods) {
        swc->carryingDangerousGoods = new DangerousGoodsExtended();
    }
    if (!swc->carryingDangerousGoods->companyName) {
        swc->carryingDangerousGoods->companyName = new PhoneNumber_t();
    }
    setOctetString(swc->carryingDangerousGoods->companyName, companyName, companyNameSize);

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMStationaryVehicleContainerNumberOfOccupants(int stationID, int sequenceNumber, int numberOfOccupants)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer* swc;
    int err = getStationaryVehicleContainerInitialized(stationID, sequenceNumber, &swc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->numberOfOccupants) {
        swc->numberOfOccupants = new NumberOfOccupants_t();
    }
    *swc->numberOfOccupants = numberOfOccupants;

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMStationaryVehicleContainerVehicleIdentification(int stationID, int sequenceNumber, uint8_t *wMInumber, int wMInumberSize, uint8_t *vDS, int vDSSize)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer* swc;
    int err = getStationaryVehicleContainerInitialized(stationID, sequenceNumber, &swc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->vehicleIdentification) {
        swc->vehicleIdentification = new VehicleIdentification();
        swc->vehicleIdentification->vDS = new IA5String_t();
        swc->vehicleIdentification->wMInumber = new IA5String_t();
    }
    setOctetString(swc->vehicleIdentification->vDS, vDS, vDSSize);
    setOctetString(swc->vehicleIdentification->wMInumber, wMInumber, wMInumberSize);

    databaseLockDENM_.unlock();
    return 0;
}

int setDENMStationaryVehicleContainerEnergyStorageType(int stationID, int sequenceNumber, int energyStorageType)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer* swc;
    int err = getStationaryVehicleContainerInitialized(stationID, sequenceNumber, &swc);
    if (err) { 
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->energyStorageType) {
        swc->energyStorageType = new EnergyStorageType_t();
    }
    setBitString(swc->energyStorageType, (uint8_t*)(&energyStorageType), sizeof(int));

    databaseLockDENM_.unlock();
    return 0;
}



void getDENMHeader(DENM_t *denm, int *protocolVersion, int *messageID) 
{
    if (protocolVersion) {
        *protocolVersion = denm->header.protocolVersion;        
    }
    if (messageID) {
        *messageID = denm->header.messageID;
    }
}

int getDENMHeader(int stationID, int sequenceNumber, int *protocolVersion, int *messageID) 
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    getDENMHeader(denm, protocolVersion, messageID);

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMManagementContainer(DENM_t* denm, int* detectionTime, int* referenceTime, int* termination, int* relevanceDistance, 
    int* relevanceTrafficDirection, int* validityDuration, int* transmissionInterval, int* stationType)
{
    getTimestamp(&denm->denm.management.detectionTime, detectionTime);
    getTimestamp(&denm->denm.management.referenceTime, referenceTime);

    if (termination) {
        *termination = *denm->denm.management.termination;
    }
    if (relevanceDistance) {
        *relevanceDistance = *denm->denm.management.relevanceDistance;
    }
    if (relevanceTrafficDirection) {
        *relevanceTrafficDirection = *denm->denm.management.relevanceTrafficDirection;
    }
    if (validityDuration) {
        *validityDuration = *denm->denm.management.validityDuration;
    }
    if (transmissionInterval) {
        *transmissionInterval = *denm->denm.management.transmissionInterval;
    }
    if (stationType) {
        *stationType = denm->denm.management.stationType;
    }
    return 0;
}

int getDENMManagementContainer(int stationID, int sequenceNumber, int* detectionTime, int* referenceTime, int* termination, 
    int* relevanceDistance, int* relevanceTrafficDirection, int* validityDuration, int* transmissionInterval, int* stationType)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getDENMManagementContainer(denm, detectionTime, referenceTime, termination, relevanceDistance, 
        relevanceTrafficDirection, validityDuration, transmissionInterval, stationType);

    databaseLockDENM_.unlock();
    return ret;
}

int getDENMManagementContainerEventPosition(DENM_t* denm, int* latitude, int* longitude, 
    int* posConfEllSemiMajorConf, int* posConfEllSemiMinorConf, int* posConfEllSemiMajorOri, int* altitudeValue, int* altitudeConf)
{
    if (latitude != nullptr) {
        *latitude = denm->denm.management.eventPosition.latitude;
    }
    if (longitude != nullptr) {
        *longitude = denm->denm.management.eventPosition.longitude;
    }
    if (posConfEllSemiMajorConf != nullptr) {
        *posConfEllSemiMajorConf = denm->denm.management.eventPosition.positionConfidenceEllipse.semiMajorConfidence;
    }
    if (posConfEllSemiMinorConf != nullptr) {
        *posConfEllSemiMinorConf = denm->denm.management.eventPosition.positionConfidenceEllipse.semiMinorConfidence;
    }
    if (posConfEllSemiMajorOri != nullptr) {
        *posConfEllSemiMajorOri = denm->denm.management.eventPosition.positionConfidenceEllipse.semiMajorOrientation;
    }
    if (altitudeValue != nullptr) {
        *altitudeValue = denm->denm.management.eventPosition.altitude.altitudeValue;
    }
    if (altitudeConf != nullptr) {
        *altitudeConf = denm->denm.management.eventPosition.altitude.altitudeConfidence;
    }
    return 0;
}

int getDENMManagementContainerEventPosition(int stationID, int sequenceNumber, int* latitude, int* longitude, 
    int* posConfEllSemiMajorConf, int* posConfEllSemiMinorConf, int* posConfEllSemiMajorOri, int* altitudeValue, int* altitudeConf)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getDENMManagementContainerEventPosition(denm, latitude, longitude, posConfEllSemiMajorConf, 
        posConfEllSemiMinorConf, posConfEllSemiMajorOri, altitudeValue, altitudeConf);

    databaseLockDENM_.unlock();
    return ret;
}

int getSituationContainer(DENM_t* denm, SituationContainer **sc) 
{
    *sc = denm->denm.situation;
    if (!*sc) {
        std::stringstream ss;
        ss << "There is no SituationContainer present in this message. (StationID=" << denm->header.stationID 
            << ", SequenceNumber=" << denm->denm.management.actionID.sequenceNumber << ")" << std::endl;
        setLastErrMsg(ss.str().c_str(), (int)ss.str().size());
        return ERR_NULL;
    }
    return 0;
}

int getDENMSituationContainer(DENM_t* denm, int *informationQuality, int *causeCode, int *subCauseCode)
{
    SituationContainer *sc;
    int scRet = getSituationContainer(denm, &sc);
    if (scRet != 0) 
    {
        return scRet;
    }

    if (informationQuality) {
        *informationQuality = sc->informationQuality;
    }
    if (causeCode) {
        *causeCode = sc->eventType.causeCode;
    }
    if (subCauseCode) {
        *subCauseCode = sc->eventType.subCauseCode;
    }
    return 0;
}

int getDENMSituationContainer(int stationID, int sequenceNumber, int *informationQuality, int *causeCode, int *subCauseCode)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getDENMSituationContainer(denm, informationQuality, causeCode, subCauseCode);

    databaseLockDENM_.unlock();
    return ret;
}

int getDENMSituationContainerLinkedCause(DENM_t *denm, int *causeCode, int *subCauseCode)
{
    SituationContainer *sc;
    int scRet = getSituationContainer(denm, &sc);
    if (scRet != 0) 
    {
        return scRet;
    }

    CauseCode *cc = sc->linkedCause;
    if (!cc) {
        std::stringstream ss;
        ss << "There is no LinkedCauseCode present in this SituationContainer. (StationID=" << denm->header.stationID 
            << ", SequenceNumber=" << denm->denm.management.actionID.sequenceNumber << ")" << std::endl;
        setLastErrMsg(ss.str().c_str(), (int)ss.str().size());
        return ERR_NULL;
    }

    *causeCode = cc->causeCode;
    *subCauseCode = cc->subCauseCode;

    return 0;
}

int getDENMSituationContainerLinkedCause(int stationID, int sequenceNumber, int *causeCode, int *subCauseCode)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getDENMSituationContainerLinkedCause(denm, causeCode, subCauseCode);

    databaseLockDENM_.unlock();
    return ret;
}

int getDENMSituationContainerEventHistory(DENM_t *denm, int *eventHistory, int eventHistorySize, int *actualEventHistorySize)
{
    SituationContainer *sc;
    int scRet = getSituationContainer(denm, &sc);
    if (scRet != 0) 
    {
        return scRet;
    }

    EventHistory *eh = sc->eventHistory;
    if (!eh) {
        std::stringstream ss;
        ss << "There is no EventHistory present in this SituationContainer. (StationID=" << denm->header.stationID 
            << ", SequenceNumber=" << denm->denm.management.actionID.sequenceNumber << ")" << std::endl;
        setLastErrMsg(ss.str().c_str(), (int)ss.str().size());
        return ERR_NULL;
    }

    if (eventHistory) 
    {
        int len = eh->list.count;
        int cpySize = (std::min)((int)(eventHistorySize / 5), len);
        for (int i = 0; i < cpySize; i++)
        {
            eventHistory[i*5 + 0] = eh->list.array[len - 1 - i]->eventPosition.deltaLatitude;
            eventHistory[i*5 + 1] = eh->list.array[len - 1 - i]->eventPosition.deltaLongitude;
            eventHistory[i*5 + 2] = eh->list.array[len - 1 - i]->eventPosition.deltaAltitude;
            eventHistory[i*5 + 3] = *eh->list.array[len - 1 - i]->eventDeltaTime;
            eventHistory[i*5 + 4] = eh->list.array[len - 1 - i]->informationQuality;
        }
    }
    if (actualEventHistorySize) {
        *actualEventHistorySize = eh->list.count * 5;
    }
    return 0;
}

int getDENMSituationContainerEventHistory(int stationID, int sequenceNumber, int *eventHistory, int eventHistorySize, int *actualEventHistorySize)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getDENMSituationContainerEventHistory(denm, eventHistory, eventHistorySize, actualEventHistorySize);

    databaseLockDENM_.unlock();
    return ret;
}

int getLocationContainer(DENM_t* denm, LocationContainer **lc) 
{
    *lc = denm->denm.location;
    if (!*lc) {
        std::stringstream ss;
        ss << "There is no LocationContainer present in this message. (StationID=" << denm->header.stationID 
            << ", SequenceNumber=" << denm->denm.management.actionID.sequenceNumber << ")" << std::endl;
        setLastErrMsg(ss.str().c_str(), (int)ss.str().size());
        return ERR_NULL;
    }
    return 0;
}

int getDENMLocationContainerTrace(DENM_t *denm, int traceIndex, int* trace, int traceBufferLength, int *actualTraceLength)
{
    if (!trace) 
    {
        return ERR_ARG_NULL;
    }

    LocationContainer *lc;
    int lcRet = getLocationContainer(denm, &lc);
    if (lcRet != 0) 
    {
        return lcRet;
    }

    if (traceIndex >= lc->traces.list.count) 
    {
        std::stringstream ss;
        ss << "Trace index '" << traceIndex << "' is out of range. Threr are only '" << lc->traces.list.count << "' traces in the message.";
        setLastErrMsg(ss.str().c_str(), (int)ss.str().size());
        return ERR_INDEX_OUT_OF_RANGE;
    }

    int len = (std::min)((int)(traceBufferLength / 4), lc->traces.list.array[traceIndex]->list.count);
    for (int i = 0; i < len; i++)
    {
        trace[i*4 + 0] = lc->traces.list.array[traceIndex]->list.array[i]->pathPosition.deltaLatitude;
        trace[i*4 + 1] = lc->traces.list.array[traceIndex]->list.array[i]->pathPosition.deltaLongitude;
        trace[i*4 + 2] = lc->traces.list.array[traceIndex]->list.array[i]->pathPosition.deltaAltitude;
        trace[i*4 + 3] = *lc->traces.list.array[traceIndex]->list.array[i]->pathDeltaTime;
    }

    if (actualTraceLength) {
        *actualTraceLength = lc->traces.list.array[traceIndex]->list.count * 4;
    }
    
    return 0;
}

int getDENMLocationContainerTrace(int stationID, int sequenceNumber, int traceIndex, int* trace, int traceBufferLength, int *actualTraceLength)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getDENMLocationContainerTrace(denm, traceIndex, trace, traceBufferLength, actualTraceLength);

    databaseLockDENM_.unlock();
    return ret;
}

int getDENMLocationContainerSpeed(DENM_t *denm, int *speedValue, int *speedConfidence)
{
    LocationContainer *lc;
    int lcRet = getLocationContainer(denm, &lc);
    if (lcRet != 0) 
    {
        return lcRet;
    }

    if (!lc->eventSpeed)
    {        
        return ERR_NULL;
    }

    if (speedValue) 
    {
        *speedValue = lc->eventSpeed->speedValue;
    }
    if (speedConfidence)
    {
        *speedConfidence = lc->eventSpeed->speedConfidence;
    }

    return 0;
}

int getDENMLocationContainerSpeed(int stationID, int sequenceNumber, int *speedValue, int *speedConfidence)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getDENMLocationContainerSpeed(denm, speedValue, speedConfidence);

    databaseLockDENM_.unlock();
    return ret;
}

int getDENMLocationContainerHeading(DENM_t *denm, int *headingValue, int *headingConfidence)
{
    LocationContainer *lc;
    int lcRet = getLocationContainer(denm, &lc);
    if (lcRet != 0) 
    {
        return lcRet;
    }

    if (!lc->eventPositionHeading)
    {        
        return ERR_NULL;
    }

    if (headingValue) 
    {
        *headingValue = lc->eventPositionHeading->headingValue;
    }
    if (headingConfidence)
    {
        *headingConfidence = lc->eventPositionHeading->headingConfidence;
    }
    return 0;
}

int getDENMLocationContainerHeading(int stationID, int sequenceNumber, int *headingValue, int *headingConfidence)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getDENMLocationContainerHeading(denm, headingValue, headingConfidence);

    databaseLockDENM_.unlock();
    return ret;
}

int getDENMLocationContainerRoadType(DENM_t *denm, int *roadType)
{
    LocationContainer *lc;
    int lcRet = getLocationContainer(denm, &lc);
    if (lcRet != 0) 
    {
        return lcRet;
    }

    if (!lc->roadType)
    {        
        return ERR_NULL;
    }

    if (roadType) 
    {
        *roadType = *lc->roadType;
    }
    return 0;
}

int getDENMLocationContainerRoadType(int stationID, int sequenceNumber, int *roadType)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getDENMLocationContainerRoadType(denm, roadType);

    databaseLockDENM_.unlock();
    return ret;
}

int getDENMAlacarteContainer(int stationID, int sequenceNumber, int *lanePosition, int *externalTemperature, int *positioningSolution)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    AlacarteContainer *ac = denm->denm.alacarte;
    if (!ac) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }

    if (lanePosition && ac->lanePosition) { *lanePosition = *ac->lanePosition; }
    if (externalTemperature && ac->externalTemperature) { *externalTemperature = *ac->externalTemperature; }
    if (positioningSolution && ac->positioningSolution) { *positioningSolution = *ac->positioningSolution; }

    databaseLockDENM_.unlock();
    return 0;
}


int getDENMImpactReductionContainer(int stationID, int sequenceNumber, int *heightLonCarrLeft, int *heightLonCarrRight, int *posLonCarrLeft, 
    int *posLonCarrRight, int *positionOfPillars, int positionOfPillarsSize, int *posCentMass, int *wheelBaseVehicle, int *turningRadius, int *posFrontAx, 
    uint8_t *positionOfOccupants, int positionOfOccupantsSize, int *vehicleMass, int *requestResponseIndication)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    AlacarteContainer *ac = denm->denm.alacarte;
    if (!ac) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }

    ImpactReductionContainer *irc = ac->impactReduction;
    if (!irc) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }

    if (heightLonCarrLeft) { *heightLonCarrLeft = irc->heightLonCarrLeft; }
    if (heightLonCarrRight) { *heightLonCarrRight = irc->heightLonCarrRight; }
    if (posLonCarrLeft) { *posLonCarrLeft = irc->posLonCarrLeft; }
    if (posLonCarrRight) { *posLonCarrRight = irc->posLonCarrRight; }
    if (posCentMass) { *posCentMass = irc->posCentMass; }
    if (wheelBaseVehicle) { *wheelBaseVehicle = irc->wheelBaseVehicle; }
    if (turningRadius) { *turningRadius = irc->turningRadius; }
    if (posFrontAx) { *posFrontAx = irc->posFrontAx; }
    if (vehicleMass) { *vehicleMass = irc->vehicleMass; }
    if (requestResponseIndication) { *requestResponseIndication = irc->requestResponseIndication; }
    if (positionOfPillars && positionOfPillarsSize > 0) {
        int mLen = (std::min)(positionOfPillarsSize, irc->positionOfPillars.list.count);
        for (int i = 0; i < mLen; i++)
        {
            positionOfPillars[i] = *irc->positionOfPillars.list.array[i];
        }        
    }
    if (positionOfOccupants && positionOfOccupantsSize > 0) {
        getBitString(&irc->positionOfOccupants, positionOfOccupants, positionOfOccupantsSize);
    }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMRaodWorksContainerExtended(int stationID, int sequenceNumber, RoadWorksContainerExtended **rwc) {
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        return ERR_MSG_NOT_FOUND;
    }

    AlacarteContainer *ac = denm->denm.alacarte;
    if (!ac) {
        return ERR_NULL;
    } 

    if (!ac->roadWorks) {
        return ERR_NULL;
    }
    *rwc = ac->roadWorks;

    return 0;
}

int getDENMRoadWorksContainerExtendedLightBarSiren(int stationID, int sequenceNumber, uint8_t *lightBarSirenInUse, int lightBarSirenInUseSize)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended *rwc;
    int err = getDENMRaodWorksContainerExtended(stationID, sequenceNumber, &rwc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->lightBarSirenInUse) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    getBitString(rwc->lightBarSirenInUse, lightBarSirenInUse, lightBarSirenInUseSize);

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMRoadWorksContainerExtendedClosedLanes(int stationID, int sequenceNumber, int *innerhardShoulderStatus, int *outerhardShoulderStatus, int *drivingLaneStatus)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended *rwc;
    int err = getDENMRaodWorksContainerExtended(stationID, sequenceNumber, &rwc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->closedLanes) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (innerhardShoulderStatus && rwc->closedLanes->innerhardShoulderStatus) { 
        *innerhardShoulderStatus = *rwc->closedLanes->innerhardShoulderStatus; 
    }
    if (outerhardShoulderStatus && rwc->closedLanes->outerhardShoulderStatus) { 
        *outerhardShoulderStatus = *rwc->closedLanes->outerhardShoulderStatus; 
    }
    getBitString(rwc->closedLanes->drivingLaneStatus, (uint8_t*)drivingLaneStatus, sizeof(int));

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMRoadWorksContainerExtendedRestriction(int stationID, int sequenceNumber, int *stationTypes, int stationTypesSize)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended *rwc;
    int err = getDENMRaodWorksContainerExtended(stationID, sequenceNumber, &rwc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->restriction) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (stationTypes && stationTypesSize > 0) { 
        int cpyLen = (std::min)(stationTypesSize, rwc->restriction->list.count);
        for (int i = 0; i < cpyLen; i++) {
            stationTypes[i] = *rwc->restriction->list.array[i];
        }
    }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMRoadWorksContainerExtendedSpeedLimit(int stationID, int sequenceNumber, int *speedLimit)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended *rwc;
    int err = getDENMRaodWorksContainerExtended(stationID, sequenceNumber, &rwc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->speedLimit) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (speedLimit) { *speedLimit = *rwc->speedLimit; }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMRoadWorksContainerExtendedIncidentIndication(int stationID, int sequenceNumber, int *causeCode, int *subCauseCode)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended *rwc;
    int err = getDENMRaodWorksContainerExtended(stationID, sequenceNumber, &rwc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->incidentIndication) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (causeCode) { *causeCode = rwc->incidentIndication->causeCode; }
    if (subCauseCode) { *subCauseCode = rwc->incidentIndication->subCauseCode; }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMRoadWorksContainerExtendedRecommendedPath(int stationID, int sequenceNumber, int *referencePositions, int referencePositionsSize)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended *rwc;
    int err = getDENMRaodWorksContainerExtended(stationID, sequenceNumber, &rwc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->recommendedPath) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (referencePositions && referencePositionsSize > 0) {
        int cpyLen = (std::min)((int)(referencePositionsSize / 7), rwc->recommendedPath->list.count);
        for (int i = 0; i < cpyLen; i++) {
            referencePositions[i*7 + 0] = rwc->recommendedPath->list.array[i]->latitude;
            referencePositions[i*7 + 1] = rwc->recommendedPath->list.array[i]->longitude;
            referencePositions[i*7 + 2] = rwc->recommendedPath->list.array[i]->positionConfidenceEllipse.semiMajorConfidence;
            referencePositions[i*7 + 3] = rwc->recommendedPath->list.array[i]->positionConfidenceEllipse.semiMinorConfidence;
            referencePositions[i*7 + 4] = rwc->recommendedPath->list.array[i]->positionConfidenceEllipse.semiMajorOrientation;
            referencePositions[i*7 + 5] = rwc->recommendedPath->list.array[i]->altitude.altitudeValue;
            referencePositions[i*7 + 6] = rwc->recommendedPath->list.array[i]->altitude.altitudeConfidence;
        }
    }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMRoadWorksContainerExtendedStartingPointSpeedLimit(int stationID, int sequenceNumber, int *deltaLatitude, int *deltaLongitude, int *deltaAltitude)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended *rwc;
    int err = getDENMRaodWorksContainerExtended(stationID, sequenceNumber, &rwc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->startingPointSpeedLimit) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (deltaLatitude) { *deltaLatitude = rwc->startingPointSpeedLimit->deltaLatitude; }
    if (deltaLongitude) { *deltaLongitude = rwc->startingPointSpeedLimit->deltaLongitude; }
    if (deltaAltitude) { *deltaAltitude = rwc->startingPointSpeedLimit->deltaAltitude; }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMRoadWorksContainerExtendedTrafficFlowRule(int stationID, int sequenceNumber, int *trafficFlowRule)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended *rwc;
    int err = getDENMRaodWorksContainerExtended(stationID, sequenceNumber, &rwc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->trafficFlowRule) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (trafficFlowRule) { *trafficFlowRule = *rwc->trafficFlowRule; }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMRoadWorksContainerExtendedReferenceDenms(int stationID, int sequenceNumber, int *actionIDs, int actionIDsSize)
{
    databaseLockDENM_.lock();
    RoadWorksContainerExtended *rwc;
    int err = getDENMRaodWorksContainerExtended(stationID, sequenceNumber, &rwc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!rwc->referenceDenms) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (actionIDs && actionIDsSize > 0) {
        int cpyLen = (std::min)((int)(actionIDsSize / 2), rwc->referenceDenms->list.count);
        for (int i = 0; i < cpyLen; i++) {
            actionIDs[i*2 + 0] = rwc->referenceDenms->list.array[i]->originatingStationID;
            actionIDs[i*2 + 1] = rwc->referenceDenms->list.array[i]->sequenceNumber;
        }
    }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMStationaryVehicleContainer(int stationID, int sequenceNumber, StationaryVehicleContainer **swc) 
{
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        return ERR_MSG_NOT_FOUND;
    }

    AlacarteContainer *ac = denm->denm.alacarte;
    if (!ac) {
        return ERR_NULL;
    } 

    if (!ac->stationaryVehicle) {
        return ERR_NULL;
    }
    *swc = ac->stationaryVehicle;

    return 0;
}

int getDENMStationaryVehicleContainerStationarySince(int stationID, int sequenceNumber, int *stationarySince)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer *swc;
    int err = getDENMStationaryVehicleContainer(stationID, sequenceNumber, &swc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->stationarySince) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (stationarySince) { *stationarySince = *swc->stationarySince; }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMStationaryVehicleContainerStationaryCause(int stationID, int sequenceNumber, int *causeCode, int *subCauseCode)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer *swc;
    int err = getDENMStationaryVehicleContainer(stationID, sequenceNumber, &swc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->stationaryCause) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (causeCode) { *causeCode = swc->stationaryCause->causeCode; }
    if (subCauseCode) { *subCauseCode = swc->stationaryCause->subCauseCode; }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMStationaryVehicleContainerCarryingDangerousGoods(int stationID, int sequenceNumber, int *dangerousGoodsType, 
    int *unNumber, int *elevatedTemperature, int *tunnelsRestricted, int *limitedQuantity)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer *swc;
    int err = getDENMStationaryVehicleContainer(stationID, sequenceNumber, &swc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->carryingDangerousGoods) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (dangerousGoodsType) { *dangerousGoodsType = swc->carryingDangerousGoods->dangerousGoodsType; }
    if (unNumber) { *unNumber = swc->carryingDangerousGoods->unNumber; }
    if (elevatedTemperature) { *elevatedTemperature = swc->carryingDangerousGoods->elevatedTemperature; }
    if (tunnelsRestricted) { *tunnelsRestricted = swc->carryingDangerousGoods->tunnelsRestricted; }
    if (limitedQuantity) { *limitedQuantity = swc->carryingDangerousGoods->limitedQuantity; }    

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode(int stationID, int sequenceNumber, 
    uint8_t *emergencyActionCode, int emergencyActionCodeSize)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer *swc;
    int err = getDENMStationaryVehicleContainer(stationID, sequenceNumber, &swc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->carryingDangerousGoods) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (!swc->carryingDangerousGoods->emergencyActionCode) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }

    if (emergencyActionCode && emergencyActionCodeSize > 0) {
        getOctetString(swc->carryingDangerousGoods->emergencyActionCode, emergencyActionCode, emergencyActionCodeSize);
    }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMStationaryVehicleContainerCarryingDangerousGoodsPhoneNumber(int stationID, int sequenceNumber, uint8_t *phoneNumber, int phoneNumberSize)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer *swc;
    int err = getDENMStationaryVehicleContainer(stationID, sequenceNumber, &swc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->carryingDangerousGoods) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (!swc->carryingDangerousGoods->phoneNumber) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }

    if (phoneNumber && phoneNumberSize > 0) {
        getOctetString(swc->carryingDangerousGoods->phoneNumber, phoneNumber, phoneNumberSize);
    }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMStationaryVehicleContainerCarryingDangerousGoodsCompanyName(int stationID, int sequenceNumber, uint8_t *companyName, int companyNameSize)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer *swc;
    int err = getDENMStationaryVehicleContainer(stationID, sequenceNumber, &swc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->carryingDangerousGoods) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (!swc->carryingDangerousGoods->companyName) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }

    if (companyName && companyNameSize > 0) {
        getOctetString(swc->carryingDangerousGoods->companyName, companyName, companyNameSize);
    }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMStationaryVehicleContainerNumberOfOccupants(int stationID, int sequenceNumber, int *numberOfOccupants)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer *swc;
    int err = getDENMStationaryVehicleContainer(stationID, sequenceNumber, &swc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->numberOfOccupants) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }
    if (numberOfOccupants) { *numberOfOccupants = *swc->numberOfOccupants; }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMStationaryVehicleContainerVehicleIdentification(int stationID, int sequenceNumber, uint8_t *wMInumber, int wMInumberSize, uint8_t *vDS, int vDSSize)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer *swc;
    int err = getDENMStationaryVehicleContainer(stationID, sequenceNumber, &swc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->vehicleIdentification) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }

    if (wMInumber && wMInumberSize > 0 && swc->vehicleIdentification->wMInumber) {
        getOctetString(swc->vehicleIdentification->wMInumber, wMInumber, wMInumberSize);
    }    
    if (vDS && vDSSize > 0 && swc->vehicleIdentification->vDS) {
        getOctetString(swc->vehicleIdentification->vDS, vDS, vDSSize);
    }

    databaseLockDENM_.unlock();
    return 0;
}

int getDENMStationaryVehicleContainerEnergyStorageType(int stationID, int sequenceNumber, int *energyStorageType)
{
    databaseLockDENM_.lock();
    StationaryVehicleContainer *swc;
    int err = getDENMStationaryVehicleContainer(stationID, sequenceNumber, &swc);
    if (err) {
        databaseLockDENM_.unlock();
        return err;
    }

    if (!swc->energyStorageType) {
        databaseLockDENM_.unlock();
        return ERR_NULL;
    }

    if (energyStorageType) {
        getBitString(swc->energyStorageType, (uint8_t*)energyStorageType, sizeof(int));
    }

    databaseLockDENM_.unlock();
    return 0;
}



int writeCallbackDENM(const void* src, size_t size, void* application_specific_key)
{
    return (int)writeCallbackBufferDENM_->write(src, (int)size, application_specific_key);
}

int encodeDENM(int stationID, int sequenceNumber, uint8_t* buffer, int size, int *actualSize)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    writeCallbackLockDENM_.lock();
    VectorBuffer* vectorBuffer = new VectorBuffer();
    writeCallbackBufferDENM_ = vectorBuffer;
    asn_enc_rval_t retVal = xer_encode(&asn_DEF_DENM, (void*)denm, XER_F_BASIC, writeCallbackDENM, NULL);
    writeCallbackBufferDENM_ = nullptr;
    writeCallbackLockDENM_.unlock();
    databaseLockDENM_.unlock();

    if (retVal.encoded == -1)
    {
        delete vectorBuffer;
        std::stringstream errMsgStream;
        std::string xmlTag(retVal.failed_type->xml_tag);
        errMsgStream 
            << "DENM Encoding failed (Code=" << retVal.failed_type->name << "). " 
            << "This is probably due to an invalid value of property '" << retVal.failed_type->xml_tag
            << "' in the message of Station '" 
            << stationID << "' and sequence number '" << sequenceNumber << "'." << std::endl;
        setLastErrMsg(errMsgStream.str().c_str(), (int)errMsgStream.str().size());
        std::cout << errMsgStream.str();
        return ERR_ENCODE;
    }

    int required_buffer_size = (int)vectorBuffer->size();
    if (size < required_buffer_size)
    {
        delete vectorBuffer;
        return ERR_BUFFER_OVERFLOW;
    }

    ((char*)buffer)[required_buffer_size + 1] = '\0';
    int copiedBytes = (int)vectorBuffer->copy(buffer, required_buffer_size);
    if (actualSize) {
        *actualSize = copiedBytes;
    }
    delete vectorBuffer;

    return copiedBytes;
}

int decodeDENM(int* stationID, int* sequenceNumber, uint8_t* buffer, int size)
{
    databaseLockDENM_.lock();

    DENM_t* denm = nullptr;
    asn_dec_rval_t retVal;
    asn_codec_ctx_t opt_codec_ctx{};
    opt_codec_ctx.max_stack_size = 0;
    retVal = xer_decode(&opt_codec_ctx, &asn_DEF_DENM, (void**)&denm, buffer, size);

    if (retVal.code == asn_dec_rval_code_e::RC_FAIL)
    {
        databaseLockDENM_.unlock();
        return ERR_DECODE;
    }

    *stationID = denm->header.stationID;
    *sequenceNumber = denm->denm.management.actionID.sequenceNumber;
    ActionID aID(denm->denm.management.actionID);

    auto dbIt = getDENMiterator(aID);
    if (dbIt != databaseDENM_.end()) {
        ASN_STRUCT_FREE(asn_DEF_DENM, dbIt->second);
        dbIt->second = denm;
    }
    else {
        databaseDENM_.insert(std::pair<ActionId, DENM_t*>(aID, denm));
    }
    databaseLockDENM_.unlock();

    return retVal.code;
}

int setDENMTransmissionSource(int stationID, int sequenceNumber)
{
    DENMTransmitter::getInstance().setMessageSource(stationID, sequenceNumber);
    return 0;
}

#ifdef __cplusplus
};
#endif