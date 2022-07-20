#pragma once

#include <level_cam.h>
#include <level_denm.h>
#include <level_common.h>
#include <level_error.h>

#ifdef __cplusplus
/**
 * @brief Library namespace
 *
 */
namespace level {
extern "C" {
#endif

const char* SHARED_EXPORT getVersion();

#ifdef __cplusplus
}
};
#endif