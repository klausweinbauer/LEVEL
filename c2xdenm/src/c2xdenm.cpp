#include <c2xdenm.h>
#include <DENMReceiver.hpp>
#include <DENMTransmitter.hpp>
#include <iostream>
#include <map>
#include <DENM.h>
#include <VectorBuffer.hpp>
#include <sstream>

namespace c2x {

typedef struct ActionID {
    int stationID_ = 0;
    int sequenceNumber_ = 0;
    ActionID(ActionID_t &aID) : stationID_(aID.originatingStationID), sequenceNumber_(aID.sequenceNumber) {}
    friend bool operator<(const ActionID& a1, const ActionID& a2) {
        return (((uint64_t)a1.stationID_ << (uint64_t)32) + (uint64_t)a1.sequenceNumber_) < 
            (((uint64_t)a2.stationID_ << (uint64_t)32) + (uint64_t)a2.sequenceNumber_);
    }
} ActionID;

static std::map<ActionID, DENM_t*> databaseDENM_;
static std::mutex databaseLockDENM_;
static c2x::Buffer* writeCallbackBufferDENM_;
static std::mutex writeCallbackLockDENM_;

std::map<ActionID, DENM_t*>::iterator getDENMiterator(int stationID, int sequenceNumber) {
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

std::map<ActionID, DENM_t*>::iterator getDENMiterator(ActionID_t aID) {
    return getDENMiterator(aID.originatingStationID, aID.sequenceNumber);
}

std::map<ActionID, DENM_t*>::iterator getDENMiterator(ActionID aID) {
    return getDENMiterator(aID.stationID_, aID.sequenceNumber_);
}

DENM* getDENM(ActionID_t aID) {
    return getDENM(aID.originatingStationID, aID.sequenceNumber);
}

DENM* getDENM(ActionID aID) {
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

int startDENMReceiver(int port)
{
    try
    {
        DENMReceiver::getInstance().start(port);
    }
    catch (const std::exception& ex)
    {
        std::cout << "[ERROR] " << ex.what() << std::endl;
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
        std::cout << "[ERROR] " << ex.what() << std::endl;
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

    ActionID aID(denm->denm.management.actionID);
    databaseDENM_.insert(std::pair<ActionID, DENM_t*>(aID, denm));
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

    ActionID aID = it->first;
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

    if (!denm->denm.management.detectionTime.buf) {
        denm->denm.management.detectionTime.buf = (uint8_t*)malloc(sizeof(int));
    }
    if (!denm->denm.management.detectionTime.buf)
    {
        databaseLockDENM_.unlock();
        return ERR_ALLOC_FAILED;
    }
    memcpy(denm->denm.management.detectionTime.buf, (uint8_t*)&detectionTime, sizeof(int));
    denm->denm.management.detectionTime.size = sizeof(int);

    if (!denm->denm.management.referenceTime.buf) {
        denm->denm.management.referenceTime.buf = (uint8_t*)malloc(sizeof(int));
    }
    if (!denm->denm.management.referenceTime.buf)
    {
        databaseLockDENM_.unlock();
        return ERR_ALLOC_FAILED;
    }
    memcpy(denm->denm.management.referenceTime.buf, (uint8_t*)&referenceTime, sizeof(int));
    denm->denm.management.referenceTime.size = sizeof(int);

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
        return 0;
    }
    LocationContainer* lc = denm->denm.location;

    asn_sequence_empty(&lc->traces);

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
    if (detectionTime && denm->denm.management.detectionTime.buf) {
        int cpySize = (std::min)((int)sizeof(int), denm->denm.management.detectionTime.size);
        memcpy((uint8_t*)detectionTime, denm->denm.management.detectionTime.buf, cpySize);
    }

    if (referenceTime && denm->denm.management.referenceTime.buf) {
        int cpySize = (std::min)((int)sizeof(int), denm->denm.management.referenceTime.size);
        memcpy((uint8_t*)referenceTime, denm->denm.management.referenceTime.buf, cpySize);
    }

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
        ss << "There is no SituationContainer present in this message. (StationID=" << denm->header.stationID << ", SequenceNumber=" << denm->denm.management.actionID.sequenceNumber << ")" << std::endl;
        setLastErrMsg(ss.str().c_str(), ss.str().size());
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
        ss << "There is no LinkedCauseCode present in this SituationContainer. (StationID=" << denm->header.stationID << ", SequenceNumber=" << denm->denm.management.actionID.sequenceNumber << ")" << std::endl;
        setLastErrMsg(ss.str().c_str(), ss.str().size());
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
        ss << "There is no EventHistory present in this SituationContainer. (StationID=" << denm->header.stationID << ", SequenceNumber=" << denm->denm.management.actionID.sequenceNumber << ")" << std::endl;
        setLastErrMsg(ss.str().c_str(), ss.str().size());
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
        ss << "There is no LocationContainer present in this message. (StationID=" << denm->header.stationID << ", SequenceNumber=" << denm->denm.management.actionID.sequenceNumber << ")" << std::endl;
        setLastErrMsg(ss.str().c_str(), ss.str().size());
        return ERR_NULL;
    }
    return 0;
}

int getDENMLocationContainerTrace(DENM_t *denm, int traceIndex, int* trace, int traceBufferLength)
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
        setLastErrMsg(ss.str().c_str(), ss.str().size());
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
    
    return 0;
}

int getDENMLocationContainerTrace(int stationID, int sequenceNumber, int traceIndex, int* trace, int traceBufferLength)
{
    databaseLockDENM_.lock();
    DENM_t* denm = getDENM(stationID, sequenceNumber);
    if (!denm) {
        databaseLockDENM_.unlock();
        return ERR_MSG_NOT_FOUND;
    }

    int ret = getDENMLocationContainerTrace(denm, traceIndex, trace, traceBufferLength);

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
        databaseDENM_.insert(std::pair<ActionID, DENM_t*>(aID, denm));
    }
    databaseLockDENM_.unlock();

    return retVal.code;
}

int setDENMTransmissionSource(int stationID, int sequenceNumber)
{
    DENMTransmitter::getInstance().setMessageSource(stationID, sequenceNumber);
    return 0;
}

};