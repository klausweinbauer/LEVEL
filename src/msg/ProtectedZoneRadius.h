/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "../../TS102894-2v131-CDD.asn"
 */

#ifndef	_ProtectedZoneRadius_H_
#define	_ProtectedZoneRadius_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ProtectedZoneRadius {
	ProtectedZoneRadius_oneMeter	= 1
} e_ProtectedZoneRadius;

/* ProtectedZoneRadius */
typedef long	 ProtectedZoneRadius_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ProtectedZoneRadius;
asn_struct_free_f ProtectedZoneRadius_free;
asn_struct_print_f ProtectedZoneRadius_print;
asn_constr_check_f ProtectedZoneRadius_constraint;
ber_type_decoder_f ProtectedZoneRadius_decode_ber;
der_type_encoder_f ProtectedZoneRadius_encode_der;
xer_type_decoder_f ProtectedZoneRadius_decode_xer;
xer_type_encoder_f ProtectedZoneRadius_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _ProtectedZoneRadius_H_ */
#include <asn_internal.h>
