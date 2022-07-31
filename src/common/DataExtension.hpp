#pragma once

#include <Exception.hpp>
#include <cstdlib>

namespace level {

// Definitions for reference property
#define DEF_GET_RP(type, parentType, property, name)                           \
  inline type *get##name(parentType *parent) { return &parent->property; }     \
  inline type *get##name(parentType &parent) { return &parent.property; }

#define DEF_SET_RP(type, parentType, property, name)                           \
  inline void set##name(parentType *parent, type value) {                      \
    parent->property = value;                                                  \
  }                                                                            \
  inline void set##name(parentType &parent, type value) {                      \
    parent.property = value;                                                   \
  }

#define DEF_RP(type, parentType, property, name)                               \
  DEF_GET_RP(type, parentType, property, name)                                 \
  DEF_SET_RP(type, parentType, property, name)

// Pointer property
#define DEF_GET_PP(type, parentType, property, name)                           \
  inline type *get##name(parentType *parent) { return parent->property; }      \
  inline type *get##name(parentType &parent) { return parent.property; }

#define DEF_SET_PP(type, parentType, property, name)                           \
  inline void set##name(parentType *parent, type value) {                      \
    if (!parent->property) {                                                   \
      parent->property = (type *)calloc(1, sizeof(type));                      \
    }                                                                          \
    *parent->property = value;                                                 \
  }                                                                            \
  inline void set##name(parentType &parent, type value) {                      \
    if (!parent.property) {                                                    \
      parent.property = (type *)calloc(1, sizeof(type));                       \
    }                                                                          \
    *parent.property = value;                                                  \
  }

#define DEF_CLEAR_PP(type, parentType, property, name)                         \
  inline void clear##name(parentType *parent) {                                \
    if (parent->property) {                                                    \
      ASN_STRUCT_FREE(asn_DEF_##type, parent->property);                       \
      parent->property = nullptr;                                              \
    }                                                                          \
  }                                                                            \
  inline void clear##name(parentType &parent) {                                \
    if (parent.property) {                                                     \
      ASN_STRUCT_FREE(asn_DEF_##type, parent.property);                        \
      parent.property = nullptr;                                               \
    }                                                                          \
  }

#define DEF_PP(type, parentType, property, name)                               \
  DEF_GET_PP(type, parentType, property, name)                                 \
  DEF_SET_PP(type, parentType, property, name)                                 \
  DEF_CLEAR_PP(type, parentType, property, name)

} // namespace level