#include <CAM.hpp>

namespace c2x::cam {

CAM::CAM(int stationID) : _cam(new CAM_t()) {
  _cam->header.stationID = stationID;
}

CAM::~CAM() { ASN_STRUCT_FREE(asn_DEF_CAM, _cam); }

CAM::CAM(CAM &&cam) : _cam(nullptr) {
  _cam = cam._cam;
  cam._cam = nullptr;
}

CAM &CAM::operator=(CAM &&cam) {
  if (this != &cam) {
    ASN_STRUCT_FREE(asn_DEF_CAM, _cam);
    _cam = cam._cam;
    cam._cam = nullptr;
  }
  return *this;
}

CAM_t *CAM::operator->() { return _cam; }

} // namespace c2x::cam
