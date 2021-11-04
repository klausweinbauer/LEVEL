/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "../../TS102894-2v131-CDD.asn"
 */

#ifndef	_ProtectedZoneType_H_
#define	_ProtectedZoneType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ProtectedZoneType {
	ProtectedZoneType_permanentCenDsrcTolling	= 0,
	/*
	 * Enumeration is extensible
	 */
	ProtectedZoneType_temporaryCenDsrcTolling	= 1
} e_ProtectedZoneType;

/* ProtectedZoneType */
typedef long	 ProtectedZoneType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ProtectedZoneType;
asn_struct_free_f ProtectedZoneType_free;
asn_struct_print_f ProtectedZoneType_print;
asn_constr_check_f ProtectedZoneType_constraint;
ber_type_decoder_f ProtectedZoneType_decode_ber;
der_type_encoder_f ProtectedZoneType_encode_der;
xer_type_decoder_f ProtectedZoneType_decode_xer;
xer_type_encoder_f ProtectedZoneType_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _ProtectedZoneType_H_ */
#include <asn_internal.h>
