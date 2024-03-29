/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "../../TS102894-2v131-CDD.asn"
 */

#ifndef	_AltitudeValue_H_
#define	_AltitudeValue_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum AltitudeValue {
	AltitudeValue_referenceEllipsoidSurface	= 0,
	AltitudeValue_oneCentimeter	= 1,
	AltitudeValue_unavailable	= 800001
} e_AltitudeValue;

/* AltitudeValue */
typedef long	 AltitudeValue_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AltitudeValue;
asn_struct_free_f AltitudeValue_free;
asn_struct_print_f AltitudeValue_print;
asn_constr_check_f AltitudeValue_constraint;
ber_type_decoder_f AltitudeValue_decode_ber;
der_type_encoder_f AltitudeValue_encode_der;
xer_type_decoder_f AltitudeValue_decode_xer;
xer_type_encoder_f AltitudeValue_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _AltitudeValue_H_ */
#include <asn_internal.h>
