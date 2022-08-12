#include <level.h>

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

#ifndef PROJECT_VERSION
#define PROJECT_VERSION "unknown"
#endif

#ifdef __cplusplus
namespace level {
#endif

const char *getVersion() { return TO_STRING(PROJECT_VERSION); }

#ifdef __cplusplus
}
#endif