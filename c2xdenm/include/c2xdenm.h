#ifndef __c2xdenm_h
#define __c2xdenm_h

#ifdef _WIN32
#ifdef shared_EXPORTS
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT __declspec(dllimport)
#endif
#else
#define SHARED_EXPORT
#endif

#include <stdint.h>

//extern "C" void SHARED_EXPORT initdenm();
extern "C" int SHARED_EXPORT configureDENM(int port, int denm_HFC_present, int denm_LFC_present, int denm_SVC_present);

// send DENM
extern "C" int SHARED_EXPORT newDENM();
//extern "C" int SHARED_EXPORT set...();
extern "C" int SHARED_EXPORT terminateDENM();

// receive DENM
extern "C" int SHARED_EXPORT activeDENMs();
//extern "C" int SHARED_EXPORT get...(int index);

extern "C" void SHARED_EXPORT startDENMReceiver();
extern "C" void SHARED_EXPORT stopDENMReceiver();

extern "C" void SHARED_EXPORT cleanup();

#endif // __c2xdenm_h