#include <DENMEncoder.hpp>
#include <DENMWrapper.hpp>
#include <Exception.hpp>
#include <vector>

namespace level::denm {

DEREncoder DENMWrapper::_copyEncoder;

DENMWrapper::DENMWrapper() : DENMWrapper(0, 0) {}

DENMWrapper::DENMWrapper(int stationId, int sequenceNumber)
    : _denm((DENM *)calloc(1, sizeof(DENM))) {
  _denm->header.stationID = stationId;
  _denm->denm.management.actionID.originatingStationID = stationId;
  _denm->denm.management.actionID.sequenceNumber = sequenceNumber;
}

DENMWrapper::~DENMWrapper() { ASN_STRUCT_FREE(asn_DEF_DENM, _denm); }

DENMWrapper::DENMWrapper(const DENMWrapper &other) : _denm(nullptr) {
  auto data = _copyEncoder.encode(other._denm);
  _denm = _copyEncoder.decode(data);
}

DENMWrapper &DENMWrapper::operator=(DENMWrapper other) {
  swap(*this, other);
  return *this;
}

DENM *DENMWrapper::operator->() { return _denm; }
DENM &DENMWrapper::operator*() { return *_denm; }

} // namespace level::denm