#pragma once

#include <CAM.h>
#include <Buffer.hpp>

//#define PRINT_ENCODED_MSG

static c2x::Buffer* callback_buffer_;

void initCAM_BC(BasicContainer_t *bc);
void initCAM_HFC_BasicVehicle(BasicVehicleContainerHighFrequency_t* bvchf);
void initCAM_HFC_RSU(RSUContainerHighFrequency_t *rsuchf);
void initCAM_LFC_BasicVehicle(BasicVehicleContainerLowFrequency_t* bvclf);

void freeCAMPointers(CAM_t* cam);
void freeCAM_LFC_BasicVehicle(BasicVehicleContainerLowFrequency_t* bvclf);

PathPoint* allocPathPoint();
void freePathPoint(PathPoint* pathPoint);

int writeCallback(const void *src, size_t size, void *application_specific_key);
int encode(asn_TYPE_descriptor_t *type, void* struct_ptr, c2x::Buffer* buffer);
int decode(asn_TYPE_descriptor_t *type, void** struct_ptr, void* buffer, size_t buffer_size);
int decodeCAM(CAM_t** cam, void* buffer, int size);
int encodeCAM(CAM_t* cam, void* buffer, int size);