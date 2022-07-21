#pragma once

#include <level_common.h>

#ifdef __cplusplus
namespace level {

/**
 * @brief Configuration namespace
 *
 */
namespace config {

extern "C" {
#endif

static EncodingType encoding = EncodingType::DER_BER;
static unsigned short port = 5999;

#ifdef __cplusplus
}
}
};
#endif