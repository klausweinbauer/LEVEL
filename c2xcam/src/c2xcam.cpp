#include <c2xcam.h>
#include <Error.hpp>
#include <VectorBuffer.hpp>
#include <iostream>
#include <CAMService.hpp>
#include <CAMReceiver.hpp>
#include <CAMTransmitter.hpp>
#include <CAMContainer.hpp>

static unsigned int transmission_interval_ms_ = 500;
static int port_ = 1997;
static HighFrequencyContainer_PR cam_HFC_present_ = HighFrequencyContainer_PR::HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
static LowFrequencyContainer_PR cam_LFC_present_ = LowFrequencyContainer_PR::LowFrequencyContainer_PR_NOTHING;
static SpecialVehicleContainer_PR cam_SVC_present_ = SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_NOTHING;
static bool cam_is_initialized_ = false;
static int cam_LFCBV_maxPathPoints = 10;

void initCAM()
{
    if (cam_is_initialized_)
    {
        return;
    }

    CAMTransmitter::getInstance().lockCAM();

    CAM_t* cam = CAMTransmitter::getInstance().localCAM();

    cam->cam.camParameters.basicContainer.stationType = StationType_roadSideUnit;
    initCAM_BC(&cam->cam.camParameters.basicContainer);

    cam->cam.camParameters.highFrequencyContainer.present = cam_HFC_present_;
    if (cam_HFC_present_ == HighFrequencyContainer_PR::HighFrequencyContainer_PR_basicVehicleContainerHighFrequency)
    {
        initCAM_HFC_BasicVehicle(&cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency);
    }
    else
    {
        initCAM_HFC_RSU(&cam->cam.camParameters.highFrequencyContainer.choice.rsuContainerHighFrequency);
    }

    if (cam_LFC_present_ == LowFrequencyContainer_PR::LowFrequencyContainer_PR_NOTHING)
    {
        cam->cam.camParameters.lowFrequencyContainer = nullptr;
    }
    else if (cam_LFC_present_ == LowFrequencyContainer_PR::LowFrequencyContainer_PR_basicVehicleContainerLowFrequency)
    {
        cam->cam.camParameters.lowFrequencyContainer = new LowFrequencyContainer();
        cam->cam.camParameters.lowFrequencyContainer->present = cam_LFC_present_;
        initCAM_LFC_BasicVehicle(&cam->cam.camParameters.lowFrequencyContainer->choice.basicVehicleContainerLowFrequency);
    }

    if (cam_SVC_present_ == SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_NOTHING)
    {
        cam->cam.camParameters.specialVehicleContainer = nullptr;
    }
    else
    {
        cam->cam.camParameters.specialVehicleContainer = new SpecialVehicleContainer();
        cam->cam.camParameters.specialVehicleContainer->present = cam_SVC_present_;
        if (cam_SVC_present_ == SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_publicTransportContainer)
        {
            initCAM_SVC_PublicTransport(&cam->cam.camParameters.specialVehicleContainer->choice.publicTransportContainer);
        }
        else if (cam_SVC_present_ == SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_specialTransportContainer)
        {
            initCAM_SVC_SpecialTransport(&cam->cam.camParameters.specialVehicleContainer->choice.specialTransportContainer);
        }   
        else if (cam_SVC_present_ == SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_dangerousGoodsContainer)
        {
            initCAM_SVC_DangerousGoods(&cam->cam.camParameters.specialVehicleContainer->choice.dangerousGoodsContainer);
        }
        else if (cam_SVC_present_ == SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_roadWorksContainerBasic)
        {
            initCAM_SVC_RoadWorksContainer(&cam->cam.camParameters.specialVehicleContainer->choice.roadWorksContainerBasic);
        }
        else if (cam_SVC_present_ == SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_rescueContainer)
        {
            initCAM_SVC_RescueContainer(&cam->cam.camParameters.specialVehicleContainer->choice.rescueContainer);
        }
        else if (cam_SVC_present_ == SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_emergencyContainer)
        {
            initCAM_SVC_EmergencyContainer(&cam->cam.camParameters.specialVehicleContainer->choice.emergencyContainer);
        }
        else if (cam_SVC_present_ == SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_safetyCarContainer)
        {
            initCAM_SVC_SafetyCarContainer(&cam->cam.camParameters.specialVehicleContainer->choice.safetyCarContainer);
        }
    }

    CAMTransmitter::getInstance().unlockCAM();

    cam_is_initialized_ = true;
}

void configureCAM(int port, int cam_HFC_present, int cam_LFC_present, int cam_SVC_present)
{
    port_ = port;
    switch (cam_HFC_present)
    {
    // case 1 should be default
    case 2:
        cam_HFC_present_ = HighFrequencyContainer_PR::HighFrequencyContainer_PR_rsuContainerHighFrequency;
        break;
    default:
        cam_HFC_present_ = HighFrequencyContainer_PR::HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
    }

    switch (cam_LFC_present)
    {
    case 1:
        cam_LFC_present_ = LowFrequencyContainer_PR::LowFrequencyContainer_PR_basicVehicleContainerLowFrequency;
        break;
    default:
        cam_LFC_present_ = LowFrequencyContainer_PR::LowFrequencyContainer_PR_NOTHING;
    }

    switch (cam_SVC_present)
    {
    case 1:
        cam_SVC_present_ = SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_publicTransportContainer;
        break;
    case 2:
        cam_SVC_present_ = SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_specialTransportContainer;
        break;
    case 3:
        cam_SVC_present_ = SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_dangerousGoodsContainer;
        break;
    case 4:
        cam_SVC_present_ = SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_roadWorksContainerBasic;
        break;
    case 5:
        cam_SVC_present_ = SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_rescueContainer;
        break;
    case 6:
        cam_SVC_present_ = SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_emergencyContainer;
        break;
    case 7:
        cam_SVC_present_ = SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_safetyCarContainer;
        break;
    default:
        cam_SVC_present_ = SpecialVehicleContainer_PR::SpecialVehicleContainer_PR_NOTHING;
    }

    initCAM();
}

void setCAMTransmissionFrequency(double transmission_frequency)
{
    transmission_interval_ms_ = (unsigned int)(1000 / transmission_frequency);
    CAMTransmitter::getInstance().setInterval(transmission_interval_ms_);
}

int getCAMBasicContainerValues(int id, int* stationType, int* latitude, int* longitude,
    int* posConfSemiMajorC, int* posConfSemiMinorC, int* posConfSemiMajorO, int* altitudeValue, int* altitudeConf)
{
    CAMContainer::getInstance().lockContainer();

    CAM_t* cam = CAMContainer::getInstance().get(id);
    if (cam == nullptr)
    {
        CAMContainer::getInstance().unlockContainer();
        return -1;
    }

    BasicContainer_t* container = &cam->cam.camParameters.basicContainer;
    *stationType = container->stationType;
    *latitude = container->referencePosition.latitude;
    *longitude = container->referencePosition.longitude;
    *posConfSemiMajorC = container->referencePosition.positionConfidenceEllipse.semiMajorConfidence;
    *posConfSemiMinorC = container->referencePosition.positionConfidenceEllipse.semiMinorConfidence;
    *posConfSemiMajorO = container->referencePosition.positionConfidenceEllipse.semiMajorOrientation;
    *altitudeValue = container->referencePosition.altitude.altitudeValue;
    *altitudeConf = container->referencePosition.altitude.altitudeConfidence;

    CAMContainer::getInstance().unlockContainer();

    return 0;
}

int getCAMHeader(int id, int *version, int *msgID, int *stationID)
{
    CAMContainer::getInstance().lockContainer();

    CAM_t* cam = CAMContainer::getInstance().get(id);
    if (cam == nullptr)
    {
        CAMContainer::getInstance().unlockContainer();
        return -1;
    }

    *version = cam->header.protocolVersion;
    *msgID = cam->header.messageID;
    *stationID = cam->header.stationID;

    CAMContainer::getInstance().unlockContainer();

    return 0;
}

void setCAMHeader(int version, int msgID, int stationID)
{
    CAMTransmitter::getInstance().lockCAM();

    CAM_t* cam = CAMTransmitter::getInstance().localCAM();
    cam->header.protocolVersion = version;
    cam->header.messageID = msgID;
    cam->header.stationID = stationID;

    CAMTransmitter::getInstance().unlockCAM();
}

void setCAMBasicContainerValues(int stationType, int latitude, int longitude,
    int posConfSemiMajorC, int posConfSemiMinorC, int posConfSemiMajorO, int altitudeValue, int altitudeConf)
{
    CAMTransmitter::getInstance().lockCAM();

    CAM_t* cam = CAMTransmitter::getInstance().localCAM();

    BasicContainer_t* container = &cam->cam.camParameters.basicContainer;
    container->stationType = stationType;
    container->referencePosition.latitude = latitude;
    container->referencePosition.longitude = longitude;
    container->referencePosition.positionConfidenceEllipse.semiMajorConfidence = posConfSemiMajorC;
    container->referencePosition.positionConfidenceEllipse.semiMinorConfidence = posConfSemiMinorC;
    container->referencePosition.positionConfidenceEllipse.semiMajorOrientation = posConfSemiMajorO;
    container->referencePosition.altitude.altitudeValue = altitudeValue;
    container->referencePosition.altitude.altitudeConfidence = altitudeConf;

    CAMTransmitter::getInstance().unlockCAM();
}

void startCAMReceiver()
{
    try
    {
        CAMReceiver::getInstance().start(port_);
    }
    catch (const std::exception& ex)
    {
        std::cout << "[ERROR] " << ex.what() << std::endl;
    }
}

void stopCAMReceiver()
{
    CAMReceiver::getInstance().stop();
}

void startCAMTransmitter()
{
    try
    {
        CAMTransmitter::getInstance().start(port_, transmission_interval_ms_);
    }
    catch (const std::exception& ex)
    {
        std::cout << "[ERROR] " << ex.what() << std::endl;
    }
}

void stopCAMTransmitter()
{
    CAMTransmitter::getInstance().stop();
}

void cleanup()
{
    CAMContainer::getInstance().lockContainer();
    CAMContainer::getInstance().clear();
    CAMContainer::getInstance().unlockContainer();

    CAMTransmitter::getInstance().lockCAM();
    freeCAMPointers(CAMTransmitter::getInstance().localCAM());
    CAMTransmitter::getInstance().unlockCAM();
}

int getCAMHFCBasicVehicleValues(
    int stationId,
    int* headingValue, int* headingConfidence,
    int* speedValue, int* speedConfidence,
    int* driveDirection,
    int* vehicleLengthValue, int* vehicleLengthConfidence,
    int* vehicleWidth,
    int* longitudinalAccelerationValue, int* longitudinalAccelerationConfidence,
    int* curvatureValue, int* curvatureConfidence,
    int* curvatureCalculationMode,
    int* yawRateValue, int* yawRateConfidence)
{
    CAMContainer::getInstance().lockContainer();

    CAM_t* cam = CAMContainer::getInstance().get(stationId);
    if (cam == nullptr)
    {
        CAMContainer::getInstance().unlockContainer();
        return -1;
    }

    BasicVehicleContainerHighFrequency_t* c = &cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency;
    *headingValue = c->heading.headingValue;
    *headingConfidence = c->heading.headingConfidence;
    *speedValue = c->speed.speedValue;
    *speedConfidence = c->speed.speedConfidence;
    *driveDirection = c->driveDirection;
    *vehicleLengthValue = c->vehicleLength.vehicleLengthValue;
    *vehicleLengthConfidence = c->vehicleLength.vehicleLengthConfidenceIndication;
    *vehicleWidth = c->vehicleWidth;
    *longitudinalAccelerationValue = c->longitudinalAcceleration.longitudinalAccelerationValue;
    *longitudinalAccelerationConfidence = c->longitudinalAcceleration.longitudinalAccelerationConfidence;
    *curvatureValue = c->curvature.curvatureValue;
    *curvatureConfidence = c->curvature.curvatureConfidence;
    *curvatureCalculationMode = c->curvatureCalculationMode;
    *yawRateValue = c->yawRate.yawRateValue;
    *yawRateConfidence = c->yawRate.yawRateConfidence;

    CAMContainer::getInstance().unlockContainer();

    return 0;
}

void setCAMHFCBasicVehicleValues(
    int headingValue, int headingConfidence,
    int speedValue, int speedConfidence,
    int driveDirection,
    int vehicleLengthValue, int vehicleLengthConfidence,
    int vehicleWidth,
    int longitudinalAccelerationValue, int longitudinalAccelerationConfidence,
    int curvatureValue, int curvatureConfidence,
    int curvatureCalculationMode,
    int yawRateValue, int yawRateConfidence)
{
    CAMTransmitter::getInstance().lockCAM();

    CAM_t* cam = CAMTransmitter::getInstance().localCAM();

    BasicVehicleContainerHighFrequency_t* c = &cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency;
    c->heading.headingValue = headingValue;
    c->heading.headingConfidence = headingConfidence;
    c->speed.speedValue = speedValue;
    c->speed.speedConfidence = speedConfidence;
    c->driveDirection = driveDirection;
    c->vehicleLength.vehicleLengthValue = vehicleLengthValue;
    c->vehicleLength.vehicleLengthConfidenceIndication = vehicleLengthConfidence;
    c->vehicleWidth = vehicleWidth;
    c->longitudinalAcceleration.longitudinalAccelerationValue = longitudinalAccelerationValue;
    c->longitudinalAcceleration.longitudinalAccelerationConfidence = longitudinalAccelerationConfidence;
    c->curvature.curvatureValue = curvatureValue;
    c->curvature.curvatureConfidence = curvatureConfidence;
    c->curvatureCalculationMode = curvatureCalculationMode;
    c->yawRate.yawRateValue = yawRateValue;
    c->yawRate.yawRateConfidence = yawRateConfidence;

    CAMTransmitter::getInstance().unlockCAM();
}

int getCAMLFCBasicVehicleValues(int stationId, int* vehicleRole, uint8_t* exteriorLights, int* exteriorLightsSize, int* pathPoints, int* pathPointsSize)
{
    CAMContainer::getInstance().lockContainer();

    CAM_t* cam = CAMContainer::getInstance().get(stationId);
    if (cam == nullptr)
    {
        CAMContainer::getInstance().unlockContainer();
        return -1;
    }

    LowFrequencyContainer* lfc = cam->cam.camParameters.lowFrequencyContainer;
    if (lfc == nullptr)
    {
        CAMContainer::getInstance().unlockContainer();
        return -2;
    }   

    BasicVehicleContainerLowFrequency_t* c = &lfc->choice.basicVehicleContainerLowFrequency;
    if (c == nullptr)
    {
        CAMContainer::getInstance().unlockContainer();
        return -3;
    }

    *vehicleRole = c->vehicleRole;

    if (exteriorLightsSize != nullptr && exteriorLights != nullptr)
    {
        *exteriorLightsSize = c->exteriorLights.size;
        for (int i = 0; i < c->exteriorLights.size; i++)
        {
            exteriorLights[i] = c->exteriorLights.buf[i];
        }
    }

    if (pathPointsSize != nullptr && pathPoints != nullptr)
    {
        int len = c->pathHistory.list.count;
        *pathPointsSize = len;
        for (int i = 0; i < len; i++)
        {
            pathPoints[i*4 + 0] = c->pathHistory.list.array[len - 1 - i]->pathPosition.deltaLatitude;
            pathPoints[i*4 + 1] = c->pathHistory.list.array[len - 1 - i]->pathPosition.deltaLongitude;
            pathPoints[i*4 + 2] = c->pathHistory.list.array[len - 1 - i]->pathPosition.deltaAltitude;
            pathPoints[i*4 + 3] = *c->pathHistory.list.array[len - 1 - i]->pathDeltaTime;
        }
    }

    CAMContainer::getInstance().unlockContainer();
}

void setCAMLFCBasicVehicleValues(int vehicleRole, uint8_t* exteriorLights, int exteriorLightsSize)
{
    CAMTransmitter::getInstance().lockCAM();

    CAM_t* cam = CAMTransmitter::getInstance().localCAM();

    LowFrequencyContainer* lfc = cam->cam.camParameters.lowFrequencyContainer;
    if (lfc == nullptr)
    {
        CAMTransmitter::getInstance().unlockCAM();
        std::cout << "Low Frequency Container was not initialized." << std::endl;
        return;
    }

    BasicVehicleContainerLowFrequency_t* c = &lfc->choice.basicVehicleContainerLowFrequency;
    if (c == nullptr)
    {
        CAMTransmitter::getInstance().unlockCAM();
        std::cout << "Basic Vehicle Low Frequency Container was not initialized." << std::endl;
        return;
    }

    c->vehicleRole = vehicleRole;
    
    // free old exterior lights buffer
    if (c->exteriorLights.buf != nullptr)
    {
        free(c->exteriorLights.buf);
        c->exteriorLights.buf = nullptr;
    }

    c->exteriorLights.bits_unused = 0;
    c->exteriorLights.size = exteriorLightsSize;
    c->exteriorLights.buf = (uint8_t*)malloc(exteriorLightsSize);

    for (int i = 0; i < exteriorLightsSize; i++)
    {
        c->exteriorLights.buf[i] = exteriorLights[i];
    }

    CAMTransmitter::getInstance().unlockCAM();
}

int addCAMLFCBasicVehiclePathPoint(int deltaLatitude, int deltaLongitude, int deltaAltitude, int deltaTime)
{
    CAMTransmitter::getInstance().lockCAM();

    CAM_t* cam = CAMTransmitter::getInstance().localCAM();

    LowFrequencyContainer* lfc = cam->cam.camParameters.lowFrequencyContainer;
    if (lfc == nullptr)
    {
        CAMTransmitter::getInstance().unlockCAM();
        std::cout << "Low Frequency Container was not initialized." << std::endl;
        return -1;
    }

    BasicVehicleContainerLowFrequency_t* c = &lfc->choice.basicVehicleContainerLowFrequency;
    if (c == nullptr)
    {
        CAMTransmitter::getInstance().unlockCAM();
        std::cout << "Basic Vehicle Low Frequency Container was not initialized." << std::endl;
        return -2;
    }

    PathPoint* p = allocPathPoint();
    p->pathPosition.deltaLatitude = deltaLatitude;
    p->pathPosition.deltaLongitude = deltaLongitude;
    p->pathPosition.deltaAltitude = deltaAltitude;
    *p->pathDeltaTime = deltaTime;
    asn_sequence_add(&c->pathHistory.list, p);

    if (c->pathHistory.list.count > cam_LFCBV_maxPathPoints) 
    {
        asn_sequence_del(&c->pathHistory.list, 0, 1);
    }

    CAMTransmitter::getInstance().unlockCAM();

    return 0;
}

void setCAMLFCBasicVehicleMaxPathPoints(int max_points)
{
    cam_LFCBV_maxPathPoints = max_points;
}