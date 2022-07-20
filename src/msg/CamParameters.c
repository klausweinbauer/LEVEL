/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "CAM-PDU-Descriptions"
 * 	found in "../../EN302637-2v141-CAM.asn"
 */

#include "CamParameters.h"

static asn_TYPE_member_t asn_MBR_CamParameters_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct CamParameters, basicContainer),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BasicContainer,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"basicContainer"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CamParameters, highFrequencyContainer),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_HighFrequencyContainer,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"highFrequencyContainer"
		},
	{ ATF_POINTER, 2, offsetof(struct CamParameters, lowFrequencyContainer),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_LowFrequencyContainer,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"lowFrequencyContainer"
		},
	{ ATF_POINTER, 1, offsetof(struct CamParameters, specialVehicleContainer),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_SpecialVehicleContainer,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"specialVehicleContainer"
		},
};
static const ber_tlv_tag_t asn_DEF_CamParameters_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_CamParameters_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* basicContainer */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* highFrequencyContainer */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* lowFrequencyContainer */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* specialVehicleContainer */
};
static asn_SEQUENCE_specifics_t asn_SPC_CamParameters_specs_1 = {
	sizeof(struct CamParameters),
	offsetof(struct CamParameters, _asn_ctx),
	asn_MAP_CamParameters_tag2el_1,
	4,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	3,	/* Start extensions */
	5	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_CamParameters = {
	"CamParameters",
	"CamParameters",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_CamParameters_tags_1,
	sizeof(asn_DEF_CamParameters_tags_1)
		/sizeof(asn_DEF_CamParameters_tags_1[0]), /* 1 */
	asn_DEF_CamParameters_tags_1,	/* Same as above */
	sizeof(asn_DEF_CamParameters_tags_1)
		/sizeof(asn_DEF_CamParameters_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_CamParameters_1,
	4,	/* Elements count */
	&asn_SPC_CamParameters_specs_1	/* Additional specs */
};
