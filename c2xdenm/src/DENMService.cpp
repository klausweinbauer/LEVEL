#include <DENMService.hpp>
#include <Error.hpp>
#include <iostream>
#include <VectorBuffer.hpp>

void initDENM(DENM_t *denm)
{
    denm->denm.management.stationType = StationType_roadSideUnit;
}


void freeDENM(DENM_t* denm)
{
    delete denm->denm.management.detectionTime.buf;
    delete denm->denm.management.referenceTime.buf;
    delete denm->denm.management.termination;
    delete denm->denm.management.relevanceDistance;
    delete denm->denm.management.relevanceTrafficDirection;
    delete denm->denm.management.validityDuration;
    delete denm->denm.management.transmissionInterval;

    if (denm->denm.situation != nullptr)
    {

    }

    if (denm->denm.location != nullptr)
    {

    }

    if (denm->denm.alacarte != nullptr)
    {

    }

    delete denm;
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
        std::cerr << "[ERROR] Encode failed." << std::endl;
        std::cerr << "[ERROR] Code: " << retVal.failed_type->name << " " << retVal.failed_type->xml_tag << std::endl;
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

int decodeDENM(DENM_t** denm, void* buffer, int size)
{
    return decode(&asn_DEF_DENM, (void**)denm, buffer, size);
}

int encodeDENM(DENM_t* denm, void* buffer, int size)
{
    c2x::VectorBuffer vectorBuffer;
    encode(&asn_DEF_DENM, denm, (c2x::Buffer*)&vectorBuffer);

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
        std::cerr << "[ERROR] Copied wrong number of bytes from vector buffer to transmission buffer." << std::endl;
        return ERR_WRONG_SIZE;
    }
#endif

    return required_buffer_size;
}