#ifndef __DENMService_H
#define __DENMService_H

#include <DENM.h>
#include <Buffer.hpp>

static c2x::Buffer* callback_buffer_;

extern void initDENM(DENM_t *denm);

extern void freeDENM(DENM_t* denm);

int decodeDENM(DENM_t** denm, void* buffer, int size);
int encodeDENM(DENM_t* denm, void* buffer, int size);

int writeCallback(const void* src, size_t size, void* application_specific_key);
int encode(asn_TYPE_descriptor_t* type, void* struct_ptr, c2x::Buffer* buffer);
int decode(asn_TYPE_descriptor_t* type, void** struct_ptr, void* buffer, size_t buffer_size);

#endif // __DENMService_H