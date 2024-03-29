/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "../../TS102894-2v131-CDD.asn"
 */

#ifndef	_TrafficRule_H_
#define	_TrafficRule_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum TrafficRule {
	TrafficRule_noPassing	= 0,
	TrafficRule_noPassingForTrucks	= 1,
	TrafficRule_passToRight	= 2,
	TrafficRule_passToLeft	= 3
	/*
	 * Enumeration is extensible
	 */
} e_TrafficRule;

/* TrafficRule */
typedef long	 TrafficRule_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TrafficRule;
asn_struct_free_f TrafficRule_free;
asn_struct_print_f TrafficRule_print;
asn_constr_check_f TrafficRule_constraint;
ber_type_decoder_f TrafficRule_decode_ber;
der_type_encoder_f TrafficRule_encode_der;
xer_type_decoder_f TrafficRule_decode_xer;
xer_type_encoder_f TrafficRule_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _TrafficRule_H_ */
#include <asn_internal.h>
