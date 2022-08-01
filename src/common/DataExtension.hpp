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

// Definitions for pointer property
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

// Definitions for union reference property
#define DEF_GET_RPU(unionType, parentType, property, name, enum, choiceType)   \
  inline unionType *get##name(parentType *parent) {                            \
    if (parent->property.present != enum) {                                    \
      throw Exception(ERR, "Invalid choice.");                                 \
    }                                                                          \
    return &parent->property.choice.choiceType;                                \
  }                                                                            \
  inline unionType *get##name(parentType &parent) {                            \
    if (parent.property.present != enum) {                                     \
      throw Exception(ERR, "Invalid choice.");                                 \
    }                                                                          \
    return &parent.property.choice.choiceType;                                 \
  }

#define DEF_SET_RPU(type, parentType, property, name, enum)                    \
  inline void set##name(parentType *parent) {                                  \
    type newObject{};                                                          \
    newObject.present = enum;                                                  \
    parent->property = newObject;                                              \
  }                                                                            \
  inline void set##name(parentType &parent) {                                  \
    type newObject{};                                                          \
    newObject.present = enum;                                                  \
    parent.property = newObject;                                               \
  }

#define DEF_RPU(type, unionType, parentType, property, name, enum, choiceType) \
  DEF_GET_RPU(unionType, parentType, property, name, enum, choiceType)         \
  DEF_SET_RPU(type, parentType, property, name, enum)

// Definitions for union pointer property
#define DEF_GET_PPU(unionType, parentType, property, name, enum, choiceType)   \
  inline unionType *get##name(parentType *parent) {                            \
    if (!parent->property) {                                                   \
      return nullptr;                                                          \
    }                                                                          \
    if (parent->property->present != enum) {                                   \
      throw Exception(ERR, "Invalid choice.");                                 \
    }                                                                          \
    return &parent->property->choice.choiceType;                               \
  }                                                                            \
  inline unionType *get##name(parentType &parent) {                            \
    if (!parent.property) {                                                    \
      return nullptr;                                                          \
    }                                                                          \
    if (parent.property->present != enum) {                                    \
      throw Exception(ERR, "Invalid choice.");                                 \
    }                                                                          \
    return &parent.property->choice.choiceType;                                \
  }

#define DEF_SET_PPU(type, parentType, property, name, enum)                    \
  inline void set##name(parentType *parent) {                                  \
    if (!parent->property) {                                                   \
      type *newObject = (type *)calloc(1, sizeof(type));                       \
      newObject->present = enum;                                               \
      parent->property = newObject;                                            \
    } else if (parent->property->present != enum) {                            \
      throw Exception(ERR, "Other union present.");                            \
    }                                                                          \
  }                                                                            \
  inline void set##name(parentType &parent) {                                  \
    if (!parent.property) {                                                    \
      type *newObject = (type *)calloc(1, sizeof(type));                       \
      newObject->present = enum;                                               \
      parent.property = newObject;                                             \
    } else if (parent.property->present != enum) {                             \
      throw Exception(ERR, "Other union present.");                            \
    }                                                                          \
  }

#define DEF_PPU(type, unionType, parentType, property, name, enum, choiceType) \
  DEF_GET_PPU(unionType, parentType, property, name, enum, choiceType)         \
  DEF_SET_PPU(type, parentType, property, name, enum)

#define DEF_CLEAR_PPU(type, parentType, property, name)                        \
  DEF_CLEAR_PP(type, parentType, property, name)

} // namespace level