#pragma once

#include <CAM.h>
#include <RecvHandler.hpp>

namespace level::cam {

class CAMRecvHandler : public RecvHandler<CAM> {
protected:
  virtual void freeMessage(T *msg) override { ASN_STRUCT_FREE(asn_DEF_CAM); }
};

} // namespace level::cam