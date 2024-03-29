/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "../../TS102894-2v131-CDD.asn"
 */

#ifndef	_YawRateConfidence_H_
#define	_YawRateConfidence_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum YawRateConfidence {
	YawRateConfidence_degSec_000_01	= 0,
	YawRateConfidence_degSec_000_05	= 1,
	YawRateConfidence_degSec_000_10	= 2,
	YawRateConfidence_degSec_001_00	= 3,
	YawRateConfidence_degSec_005_00	= 4,
	YawRateConfidence_degSec_010_00	= 5,
	YawRateConfidence_degSec_100_00	= 6,
	YawRateConfidence_outOfRange	= 7,
	YawRateConfidence_unavailable	= 8
} e_YawRateConfidence;

/* YawRateConfidence */
typedef long	 YawRateConfidence_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_YawRateConfidence;
asn_struct_free_f YawRateConfidence_free;
asn_struct_print_f YawRateConfidence_print;
asn_constr_check_f YawRateConfidence_constraint;
ber_type_decoder_f YawRateConfidence_decode_ber;
der_type_encoder_f YawRateConfidence_encode_der;
xer_type_decoder_f YawRateConfidence_decode_xer;
xer_type_encoder_f YawRateConfidence_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _YawRateConfidence_H_ */
#include <asn_internal.h>
