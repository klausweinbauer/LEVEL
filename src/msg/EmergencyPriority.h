/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "../../TS102894-2v131-CDD.asn"
 */

#ifndef	_EmergencyPriority_H_
#define	_EmergencyPriority_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BIT_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum EmergencyPriority {
	EmergencyPriority_requestForRightOfWay	= 0,
	EmergencyPriority_requestForFreeCrossingAtATrafficLight	= 1
} e_EmergencyPriority;

/* EmergencyPriority */
typedef BIT_STRING_t	 EmergencyPriority_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EmergencyPriority;
asn_struct_free_f EmergencyPriority_free;
asn_struct_print_f EmergencyPriority_print;
asn_constr_check_f EmergencyPriority_constraint;
ber_type_decoder_f EmergencyPriority_decode_ber;
der_type_encoder_f EmergencyPriority_encode_der;
xer_type_decoder_f EmergencyPriority_decode_xer;
xer_type_encoder_f EmergencyPriority_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _EmergencyPriority_H_ */
#include <asn_internal.h>
