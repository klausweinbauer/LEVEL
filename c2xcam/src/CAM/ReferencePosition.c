/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "../../TS102894-2v131-CDD.asn"
 */

#include "ReferencePosition.h"

static asn_TYPE_member_t asn_MBR_ReferencePosition_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct ReferencePosition, latitude),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Latitude,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"latitude"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ReferencePosition, longitude),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Longitude,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"longitude"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ReferencePosition, positionConfidenceEllipse),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PosConfidenceEllipse,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"positionConfidenceEllipse"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ReferencePosition, altitude),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Altitude,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"altitude"
		},
};
static const ber_tlv_tag_t asn_DEF_ReferencePosition_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ReferencePosition_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* latitude */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* longitude */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* positionConfidenceEllipse */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* altitude */
};
static asn_SEQUENCE_specifics_t asn_SPC_ReferencePosition_specs_1 = {
	sizeof(struct ReferencePosition),
	offsetof(struct ReferencePosition, _asn_ctx),
	asn_MAP_ReferencePosition_tag2el_1,
	4,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_ReferencePosition = {
	"ReferencePosition",
	"ReferencePosition",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_ReferencePosition_tags_1,
	sizeof(asn_DEF_ReferencePosition_tags_1)
		/sizeof(asn_DEF_ReferencePosition_tags_1[0]), /* 1 */
	asn_DEF_ReferencePosition_tags_1,	/* Same as above */
	sizeof(asn_DEF_ReferencePosition_tags_1)
		/sizeof(asn_DEF_ReferencePosition_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_ReferencePosition_1,
	4,	/* Elements count */
	&asn_SPC_ReferencePosition_specs_1	/* Additional specs */
};

