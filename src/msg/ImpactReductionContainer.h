/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DENM-PDU-Descriptions"
 * 	found in "../../EN302637-3v131-DENM.asn"
 */

#ifndef	_ImpactReductionContainer_H_
#define	_ImpactReductionContainer_H_


#include <asn_application.h>

/* Including external dependencies */
#include "HeightLonCarr.h"
#include "PosLonCarr.h"
#include "PositionOfPillars.h"
#include "PosCentMass.h"
#include "WheelBaseVehicle.h"
#include "TurningRadius.h"
#include "PosFrontAx.h"
#include "PositionOfOccupants.h"
#include "VehicleMass.h"
#include "RequestResponseIndication.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ImpactReductionContainer */
typedef struct ImpactReductionContainer {
	HeightLonCarr_t	 heightLonCarrLeft;
	HeightLonCarr_t	 heightLonCarrRight;
	PosLonCarr_t	 posLonCarrLeft;
	PosLonCarr_t	 posLonCarrRight;
	PositionOfPillars_t	 positionOfPillars;
	PosCentMass_t	 posCentMass;
	WheelBaseVehicle_t	 wheelBaseVehicle;
	TurningRadius_t	 turningRadius;
	PosFrontAx_t	 posFrontAx;
	PositionOfOccupants_t	 positionOfOccupants;
	VehicleMass_t	 vehicleMass;
	RequestResponseIndication_t	 requestResponseIndication;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ImpactReductionContainer_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ImpactReductionContainer;

#ifdef __cplusplus
}
#endif

#endif	/* _ImpactReductionContainer_H_ */
#include <asn_internal.h>
