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
  try {
    auto data = _copyEncoder.encode(other._denm);
    _denm = _copyEncoder.decode(data);
  } catch (const Exception &e) {
    throw Exception(
        ERR,
        "Copying DENM failed. Only valid messages can be copied.\nError: " +
            std::string(e.getErrMsg()));
  }
}

DENMWrapper &DENMWrapper::operator=(DENMWrapper other) {
  swap(*this, other);
  return *this;
}

DENM *DENMWrapper::operator->() { return _denm; }

SituationContainer *DENMWrapper::getSituationContainer() {
  if (!_denm->denm.situation) {
    _denm->denm.situation =
        (SituationContainer *)calloc(1, sizeof(SituationContainer));
  }
  return _denm->denm.situation;
}

void DENMWrapper::clearSituationContainer() {
  ASN_STRUCT_FREE(asn_DEF_SituationContainer, _denm->denm.situation);
  _denm->denm.situation = nullptr;
}

LocationContainer *DENMWrapper::getLocationContainer() {
  if (!_denm->denm.location) {
    _denm->denm.location =
        (LocationContainer *)calloc(1, sizeof(LocationContainer));
  }
  return _denm->denm.location;
}

void DENMWrapper::clearLocationContainer() {
  ASN_STRUCT_FREE(asn_DEF_LocationContainer, _denm->denm.location);
  _denm->denm.location = nullptr;
}

AlacarteContainer *DENMWrapper::getAlacarteContainer() {
  if (!_denm->denm.alacarte) {
    _denm->denm.alacarte =
        (AlacarteContainer *)calloc(1, sizeof(AlacarteContainer));
  }
  return _denm->denm.alacarte;
}

void DENMWrapper::clearAlacarteContainer() {
  ASN_STRUCT_FREE(asn_DEF_AlacarteContainer, _denm->denm.alacarte);
  _denm->denm.alacarte = nullptr;
}

} // namespace level::denm