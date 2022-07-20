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

const SHARED_EXPORT char* getVersion();

#ifdef __cplusplus
}
};
#endif