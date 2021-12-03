#include <CAMService.hpp>
#include <Error.hpp>
#include <iostream>
#include <VectorBuffer.hpp>

void initCAM_BC(BasicContainer_t* bc)
{
    bc->stationType = StationType_unknown;
    bc->referencePosition.altitude.altitudeValue = AltitudeValue_unavailable;
    bc->referencePosition.altitude.altitudeConfidence = AltitudeConfidence_unavailable;
    bc->referencePosition.latitude = Latitude_unavailable;
    bc->referencePosition.longitude = Longitude_unavailable;
    bc->referencePosition.positionConfidenceEllipse.semiMajorConfidence = SemiAxisLength_unavailable;
    bc->referencePosition.positionConfidenceEllipse.semiMinorConfidence = SemiAxisLength_unavailable;
    bc->referencePosition.positionConfidenceEllipse.semiMajorOrientation = HeadingValue_unavailable;
}

void initCAM_HFC_BasicVehicle(BasicVehicleContainerHighFrequency_t* bvchf)
{
    bvchf->heading.headingValue = HeadingValue_unavailable;
    bvchf->heading.headingConfidence = HeadingConfidence_unavailable;
    bvchf->speed.speedValue = SpeedValue_unavailable;
    bvchf->speed.speedConfidence = SpeedConfidence_unavailable;
    bvchf->driveDirection = DriveDirection_unavailable;
    bvchf->vehicleLength.vehicleLengthValue = VehicleLengthValue_unavailable;
    bvchf->vehicleLength.vehicleLengthConfidenceIndication = VehicleLengthConfidenceIndication_unavailable;
    bvchf->vehicleWidth = VehicleWidth_unavailable;
    bvchf->longitudinalAcceleration.longitudinalAccelerationValue = LongitudinalAccelerationValue_unavailable;
    bvchf->longitudinalAcceleration.longitudinalAccelerationConfidence = AccelerationConfidence_unavailable;
    bvchf->curvature.curvatureValue = CurvatureValue_unavailable;
    bvchf->curvature.curvatureConfidence = CurvatureConfidence_unavailable;
    bvchf->curvatureCalculationMode = CurvatureCalculationMode_unavailable;
    bvchf->yawRate.yawRateValue = YawRateValue_unavailable;
    bvchf->yawRate.yawRateConfidence = YawRateConfidence_unavailable;
    bvchf->accelerationControl = nullptr;
    bvchf->lanePosition = nullptr;
    bvchf->steeringWheelAngle = nullptr;
    bvchf->lateralAcceleration = nullptr;
    bvchf->verticalAcceleration = nullptr;
    bvchf->performanceClass = nullptr;
    bvchf->cenDsrcTollingZone = nullptr;
}

void initCAM_HFC_RSU(RSUContainerHighFrequency_t* rsuchf)
{
    rsuchf->protectedCommunicationZonesRSU = nullptr;
}

void initCAM_LFC_BasicVehicle(BasicVehicleContainerLowFrequency_t* bvclf)
{
    bvclf->vehicleRole = VehicleRole::VehicleRole_default;
    bvclf->exteriorLights.bits_unused = 0;
    bvclf->exteriorLights.size = 0;
    bvclf->exteriorLights.buf = (uint8_t*)malloc(0);
    bvclf->pathHistory.list.count = 0;
    bvclf->pathHistory.list.size = 0;
    bvclf->pathHistory.list.array = nullptr;
    bvclf->pathHistory.list.free = freePathPoint;
}

int writeCallback(const void* src, size_t size, void* application_specific_key)
{
#ifdef PRINT_ENCODED_MSG
    char buffer[4098];
    memcpy(buffer, src, size);
    buffer[size] = '\0';
    std::cout << buffer;
#endif
    return (int)callback_buffer_->write(src, (int)size, application_specific_key);
}

int encode(asn_TYPE_descriptor_t* type, void* struct_ptr, c2x::Buffer* buffer)
{
    callback_buffer_ = buffer;
    asn_enc_rval_t retVal = xer_encode(type, struct_ptr, XER_F_BASIC, writeCallback, NULL);
    callback_buffer_ = nullptr;

    if (retVal.encoded == -1)
    {
#ifdef DEBUG
        std::cerr << "Encode failed." << std::endl;
        std::cerr << "Code: " << retVal.failed_type->name << " " << retVal.failed_type->xml_tag << std::endl;
#endif
        return ERR_ENCODE;
    }
    return 0;
}

int decode(asn_TYPE_descriptor_t* type, void** struct_ptr, void* buffer, size_t buffer_size)
{
    asn_dec_rval_t retVal;

    asn_codec_ctx_t opt_codec_ctx{};
    opt_codec_ctx.max_stack_size = 0;
    retVal = xer_decode(&opt_codec_ctx, type, struct_ptr, buffer, buffer_size);
    return retVal.code;
}

int decodeCAM(CAM_t** cam, void* buffer, int size)
{
    return decode(&asn_DEF_CAM, (void**)cam, buffer, size);
}

int encodeCAM(CAM_t* cam, void* buffer, int size)
{
    c2x::VectorBuffer vectorBuffer;
    encode(&asn_DEF_CAM, cam, (c2x::Buffer*)&vectorBuffer);

    size_t required_buffer_size = vectorBuffer.size();
    if (size < required_buffer_size)
    {
        return ERR_BUFFER_OVERFLOW;
    }

    ((char*)buffer)[required_buffer_size + 1] = '\0';
    size_t copiedBytes = vectorBuffer.copy(buffer, required_buffer_size);
#ifdef DEBUG
    if (copiedBytes != required_buffer_size)
    {
        std::cerr << "Copied wrong number of bytes from vector buffer to transmission buffer." << std::endl;
        return ERR_WRONG_SIZE;
    }
#endif

    return required_buffer_size;
}

void freeCAMPointers(CAM_t* cam)
{
    LowFrequencyContainer* lfc = cam->cam.camParameters.lowFrequencyContainer;
    if (lfc != nullptr)
    {
        if (lfc->present == LowFrequencyContainer_PR_basicVehicleContainerLowFrequency)
        {
            freeCAM_LFC_BasicVehicle((BasicVehicleContainerLowFrequency_t*)&lfc->choice);
        }
        free(lfc);
    }
}

void freeCAM_LFC_BasicVehicle(BasicVehicleContainerLowFrequency_t* bvclf)
{
    if (bvclf->exteriorLights.buf != nullptr)
    {
        free(bvclf->exteriorLights.buf);
        bvclf->exteriorLights.buf = nullptr;
    }

    if (bvclf->pathHistory.list.free == nullptr)
    {
        bvclf->pathHistory.list.free = freePathPoint;
    }
    asn_sequence_empty(&bvclf->pathHistory.list);
}

PathPoint* allocPathPoint() 
{
    PathPoint* pathPoint = (PathPoint*)malloc(sizeof(PathPoint));
    if (pathPoint == nullptr) {
        return nullptr;
    }
    pathPoint->pathDeltaTime = new PathDeltaTime_t();
    return pathPoint;
}

void freePathPoint(PathPoint* pathPoint)
{
    if (pathPoint == nullptr) 
    {
        return;
    }

    delete pathPoint->pathDeltaTime;
    free(pathPoint);
}