#include <c2xdenm.h>
#include <DENMReceiver.hpp>
#include <DENMTransmitter.hpp>
#include <iostream>

static int port_ = 1998;
static unsigned int transmission_interval_ms_ = 500;

int configureDENM(int port)
{
	port_ = port;
	return 0;
}

void startDENMReceiver()
{
    try
    {
        DENMReceiver::getInstance().start(port_);
    }
    catch (const std::exception& ex)
    {
        std::cout << "[ERROR] " << ex.what() << std::endl;
    }
}

void stopDENMReceiver()
{
    DENMReceiver::getInstance().stop();
}

void startDENMTransmitter()
{
    try
    {
        DENMTransmitter::getInstance().start(port_, transmission_interval_ms_);
    }
    catch (const std::exception& ex)
    {
        std::cout << "[ERROR] " << ex.what() << std::endl;
    }
}

void stopDENMTransmitter()
{
    DENMTransmitter::getInstance().stop();
}

void  cleanupDENM()
{
    DENMReceiver::getInstance().lockMsgContainer();
    DENMReceiver::getInstance().msgContainer_->clear();
    //delete DENMReceiver::getInstance().msgContainer_;
    DENMReceiver::getInstance().unlockMsgContainer();

    DENMTransmitter::getInstance().lockMsgContainer();
    DENMTransmitter::getInstance().msgContainer_->clear();
    //delete DENMTransmitter::getInstance().msgContainer_;
    DENMTransmitter::getInstance().unlockMsgContainer();
}

void setDENMTransmissionFrequency(double transmission_frequency)
{
    transmission_interval_ms_ = (unsigned int)(1000 / transmission_frequency);
    DENMTransmitter::getInstance().setInterval(transmission_interval_ms_);
}

int setActiveState(int stationId, int sequenceNumber, bool state)
{
    DENMTransmitter::getInstance().lockMsgContainer();
    int ret = DENMTransmitter::getInstance().msgContainer_->setActiveState(stationId, sequenceNumber, state);
    DENMTransmitter::getInstance().unlockMsgContainer();

    return ret;
}

int createDENM(int protocolVersion, int messageId, int stationId, int sequenceNumber)
{
    DENM_t* denm = new DENM_t();
    denm->header.messageID = messageId;
    denm->header.protocolVersion = protocolVersion;
    denm->header.stationID = stationId;
    denm->denm.management.actionID.originatingStationID = stationId;
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

    DENMTransmitter::getInstance().lockMsgContainer();
    int ret = DENMTransmitter::getInstance().msgContainer_->addOrUpdate(denm);
    DENMTransmitter::getInstance().unlockMsgContainer();

    return ret;
}

int terminateDENM(int stationId, int sequenceNumber)
{
    DENMTransmitter::getInstance().lockMsgContainer();
    int ret = DENMTransmitter::getInstance().msgContainer_->remove(stationId, sequenceNumber);
    DENMTransmitter::getInstance().unlockMsgContainer();

    return ret;
}

int setManagementContainer(
    int stationId, int sequenceNumber,
    uint8_t* detectionTime, int detectionTimeSize,
    uint8_t* referenceTime, int referenceTimeSize,
    int termination, int relevanceDistance,
    int relevanceTrafficDirection, int validityDuration,
    int transmissionInterval, int stationType
)
{
    DENMTransmitter::getInstance().lockMsgContainer();
    DENM_t* denm = DENMTransmitter::getInstance().msgContainer_->get(stationId, sequenceNumber);
    if (denm == nullptr)
    {
        DENMTransmitter::getInstance().unlockMsgContainer();
        return -1;
    }

    if (detectionTime != nullptr)
    {
        delete denm->denm.management.detectionTime.buf;
        denm->denm.management.detectionTime.buf = (uint8_t*)malloc(detectionTimeSize);
        if (denm->denm.management.detectionTime.buf != nullptr)
        {
            denm->denm.management.detectionTime.size = detectionTimeSize;
            memcpy(denm->denm.management.detectionTime.buf, detectionTime, detectionTimeSize);
        }
        else
        {
            denm->denm.management.detectionTime.size = 0;
        }
    }

    if (referenceTime != nullptr)
    {
        delete denm->denm.management.referenceTime.buf;
        denm->denm.management.referenceTime.buf = (uint8_t*)malloc(referenceTimeSize);
        if (denm->denm.management.referenceTime.buf != nullptr)
        {
            denm->denm.management.referenceTime.size = referenceTimeSize;
            memcpy(denm->denm.management.referenceTime.buf, referenceTime, referenceTimeSize);
        }
        else
        {
            denm->denm.management.referenceTime.size = referenceTimeSize;
        }
    }

    if (denm->denm.management.termination == nullptr) {
        denm->denm.management.termination = new Termination_t();
    }
    *denm->denm.management.termination = termination;
    
    if (denm->denm.management.relevanceDistance == nullptr) {
        denm->denm.management.relevanceDistance = new RelevanceDistance_t();
    }
    *denm->denm.management.relevanceDistance = relevanceDistance;

    if (denm->denm.management.relevanceTrafficDirection == nullptr) {
        denm->denm.management.relevanceTrafficDirection = new RelevanceTrafficDirection_t();
    }
    *denm->denm.management.relevanceTrafficDirection = relevanceTrafficDirection;

    if (denm->denm.management.validityDuration == nullptr) {
        denm->denm.management.validityDuration = new ValidityDuration_t();
    }
    *denm->denm.management.validityDuration = validityDuration;

    if (denm->denm.management.transmissionInterval == nullptr) {
        denm->denm.management.transmissionInterval = new TransmissionInterval_t();
    }
    *denm->denm.management.transmissionInterval = transmissionInterval;

    denm->denm.management.stationType = stationType;

    DENMTransmitter::getInstance().unlockMsgContainer();

    return 0;
}

int setManagementContainerEventPosition(
    int stationId, int sequenceNumber,
    int latitude, int longitude,
    int posConfEllSemiMajorConf, int posConfEllSemiMinorConf, int posConfEllSemiMajorOri,
    int altitudeValue, int altitudeConf
)
{
    DENMTransmitter::getInstance().lockMsgContainer();
    DENM_t* denm = DENMTransmitter::getInstance().msgContainer_->get(stationId, sequenceNumber);
    if (denm == nullptr)
    {
        DENMTransmitter::getInstance().unlockMsgContainer();
        return -1;
    }

    denm->denm.management.eventPosition.latitude = latitude;
    denm->denm.management.eventPosition.longitude = longitude;
    denm->denm.management.eventPosition.positionConfidenceEllipse.semiMajorConfidence = posConfEllSemiMajorConf;
    denm->denm.management.eventPosition.positionConfidenceEllipse.semiMinorConfidence = posConfEllSemiMinorConf;
    denm->denm.management.eventPosition.positionConfidenceEllipse.semiMajorOrientation = posConfEllSemiMajorOri;
    denm->denm.management.eventPosition.altitude.altitudeValue = altitudeValue;
    denm->denm.management.eventPosition.altitude.altitudeConfidence = altitudeConf;

    DENMTransmitter::getInstance().unlockMsgContainer();

    return 0;
}

int getManagementContainer(
    int stationId, int sequenceNumber,
    uint8_t * detectionTime, int* detectionTimeSize, int detectionTimeMaxSize,
    uint8_t * referenceTime, int* referenceTimeSize, int referenceTimeMaxSize,
    int* termination, int* relevanceDistance,
    int* relevanceTrafficDirection, int* validityDuration,
    int* transmissionInterval, int* stationType)
{
    DENMReceiver::getInstance().lockMsgContainer();
    DENM_t* denm = DENMReceiver::getInstance().msgContainer_->get(stationId, sequenceNumber);
    if (denm == nullptr)
    {
        DENMReceiver::getInstance().unlockMsgContainer();
        return -1;
    }

    if (denm->denm.management.detectionTime.buf != nullptr && detectionTime != nullptr && detectionTimeSize != nullptr)
    {
        int size = std::min(detectionTimeMaxSize, denm->denm.management.detectionTime.size);
        memcpy(detectionTime, denm->denm.management.detectionTime.buf, size);
        *detectionTimeSize = denm->denm.management.detectionTime.size;
    }

    if (denm->denm.management.referenceTime.buf != nullptr && referenceTime != nullptr && referenceTimeSize != nullptr)
    {
        int size = std::min(referenceTimeMaxSize, denm->denm.management.referenceTime.size);
        memcpy(referenceTime, denm->denm.management.referenceTime.buf, size);
        *referenceTimeSize = denm->denm.management.referenceTime.size;
    }

    if (denm->denm.management.termination != nullptr && termination != nullptr) {
        *termination = *denm->denm.management.termination;
    }
    if (denm->denm.management.relevanceDistance != nullptr && relevanceDistance != nullptr) {
        *relevanceDistance = *denm->denm.management.relevanceDistance;
    }
    if (denm->denm.management.relevanceTrafficDirection != nullptr && relevanceTrafficDirection != nullptr) {
        *relevanceTrafficDirection = *denm->denm.management.relevanceTrafficDirection;
    }
    if (denm->denm.management.validityDuration != nullptr && validityDuration != nullptr) {
        *validityDuration = *denm->denm.management.validityDuration;
    }
    if (denm->denm.management.transmissionInterval != nullptr && transmissionInterval != nullptr) {
        *transmissionInterval = *denm->denm.management.transmissionInterval;
    }
    if (stationType != nullptr)
    {
        *stationType = denm->denm.management.stationType;
    }

    DENMReceiver::getInstance().unlockMsgContainer();

    return 0;
}

int getManagementContainerEventPosition(
    int stationId, int sequenceNumber,
    int* latitude, int* longitude,
    int* posConfEllSemiMajorConf, int* posConfEllSemiMinorConf, int* posConfEllSemiMajorOri,
    int* altitudeValue, int* altitudeConf)
{
    DENMReceiver::getInstance().lockMsgContainer();
    DENM_t* denm = DENMReceiver::getInstance().msgContainer_->get(stationId, sequenceNumber);
    if (denm == nullptr)
    {
        DENMReceiver::getInstance().unlockMsgContainer();
        return -1;
    }

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

    DENMReceiver::getInstance().unlockMsgContainer();

    return 0;
}