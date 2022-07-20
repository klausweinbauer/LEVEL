/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "../../TS102894-2v131-CDD.asn"
 */

#ifndef	_StationarySince_H_
#define	_StationarySince_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum StationarySince {
	StationarySince_lessThan1Minute	= 0,
	StationarySince_lessThan2Minutes	= 1,
	StationarySince_lessThan15Minutes	= 2,
	StationarySince_equalOrGreater15Minutes	= 3
} e_StationarySince;

/* StationarySince */
typedef long	 StationarySince_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_StationarySince;
asn_struct_free_f StationarySince_free;
asn_struct_print_f StationarySince_print;
asn_constr_check_f StationarySince_constraint;
ber_type_decoder_f StationarySince_decode_ber;
der_type_encoder_f StationarySince_encode_der;
xer_type_decoder_f StationarySince_decode_xer;
xer_type_encoder_f StationarySince_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _StationarySince_H_ */
#include <asn_internal.h>