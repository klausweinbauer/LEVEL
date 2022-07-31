#pragma once

#include <Exception.hpp>
#include <cstdlib>

namespace level {

template <typename T> T *get(T **property) {
  if (!property) {
    throw Exception(ERR_ARG_NULL, "Argument 'property' is null.");
  }

  if (!*property) {
    *property = (T *)calloc(1, sizeof(T));
  }
  return *property;
}

#define DEF_ASN_CLEAR(asnDef, parentType, property, name)                      \
  inline void clear##name(parentType *parent) {                                \
    if (!parent) {                                                             \
      throw Exception(ERR_ARG_NULL, "Argument 'parent' is null.");             \
    }                                                                          \
    ASN_STRUCT_FREE(asnDef, parent->property);                                 \
    parent->property = nullptr;                                                \
  }                                                                            \
  inline void clear##name(parentType &parent) {                                \
    ASN_STRUCT_FREE(asnDef, parent.property);                                  \
    parent.property = nullptr;                                                 \
  }

#define DEF_GET_PTR_PROPERTY(type, parentType, property, name, objReferencer)  \
  inline type *get##name(parentType *obj) {                                    \
    return get<type>(&objReferencer->property);                                \
  }                                                                            \
  inline type *get##name(parentType &obj) {                                    \
    return get<type>(&objReferencer.property);                                 \
  }

#define DEF_GET_PROPERTY(type, parentType, property, name, objReferencer)      \
  inline type get##name(parentType *obj) { return objReferencer->property; }   \
  inline type get##name(parentType &obj) { return objReferencer.property; }

#define DEF_SET_PROPERTY(type, parentType, property, name, objReferencer)      \
  inline void set##name(parentType *obj, type val) {                           \
    objReferencer->property = val;                                             \
  }                                                                            \
  inline void set##name(parentType &obj, type val) {                           \
    objReferencer.property = val;                                              \
  }

#define DEF_REF(type, parentType, property, name)                              \
  DEF_GET_PTR_PROPERTY(type, parentType, property, name, obj)                  \
  DEF_ASN_CLEAR(asn_DEF_##type, parentType, property, name);

#define DEF_REFT(type, parentType, property, name)                             \
  DEF_GET_PTR_PROPERTY(type##_t, parentType, property, name, obj)              \
  DEF_ASN_CLEAR(asn_DEF_##type, parentType, property, name);

#define DEF_PROP(type, parentType, property, name)                             \
  DEF_GET_PROPERTY(type, parentType, property, name, obj)                      \
  DEF_SET_PROPERTY(type, parentType, property, name, obj)

} // namespace level