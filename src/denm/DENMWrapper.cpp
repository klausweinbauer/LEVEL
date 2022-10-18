#include <DENMEncoder.hpp>
#include <DENMWrapper.hpp>
#include <Exception.hpp>
#include <vector>

namespace level::denm {

DENMDEREncoder DENMWrapper::_copyEncoder;

DENMWrapper::DENMWrapper() : DENMWrapper(0, 0) {}

DENMWrapper::DENMWrapper(DENM *denm) : _denm(denm) {}

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

DENM *DENMWrapper::operator->() const { return _denm; }

DENM &DENMWrapper::operator*() const { return *_denm; }

DENM *DENMWrapper::get() { return _denm; }

ManagementContainer *DENMWrapper::getMC() const {
  return &_denm->denm.management;
}

SituationContainer *DENMWrapper::getSC() const { return _denm->denm.situation; }

LocationContainer *DENMWrapper::getLC() const { return _denm->denm.location; }

AlacarteContainer *DENMWrapper::getAC() const { return _denm->denm.alacarte; }

SituationContainer *DENMWrapper::setSC() {
  if (_denm->denm.situation) {
    throw Exception(ERR, "SituationContainer already set.");
  }

  _denm->denm.situation =
      (SituationContainer *)calloc(1, sizeof(SituationContainer));
  return _denm->denm.situation;
}

LocationContainer *DENMWrapper::setLC() {
  if (_denm->denm.location) {
    throw Exception(ERR, "LocationContainer already set.");
  }

  _denm->denm.location =
      (LocationContainer *)calloc(1, sizeof(LocationContainer));
  return _denm->denm.location;
}

AlacarteContainer *DENMWrapper::setAC() {
  if (_denm->denm.alacarte) {
    throw Exception(ERR, "AlacarteContainer already set.");
  }

  _denm->denm.alacarte =
      (AlacarteContainer *)calloc(1, sizeof(AlacarteContainer));
  return _denm->denm.alacarte;
}

void DENMWrapper::clearMC() {
  ManagementContainer mc{};
  _denm->denm.management = mc;
}

void DENMWrapper::clearSC() {
  ASN_STRUCT_FREE(asn_DEF_SituationContainer, _denm->denm.situation);
  _denm->denm.situation = nullptr;
}

void DENMWrapper::clearLC() {
  ASN_STRUCT_FREE(asn_DEF_LocationContainer, _denm->denm.location);
  _denm->denm.location = nullptr;
}

void DENMWrapper::clearAC() {
  ASN_STRUCT_FREE(asn_DEF_AlacarteContainer, _denm->denm.alacarte);
  _denm->denm.alacarte = nullptr;
}

} // namespace level::denm