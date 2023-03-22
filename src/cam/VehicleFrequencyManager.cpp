// Copyright 2022 Klaus Weinbauer
//
// This file is part of LEVEL.
//
// LEVEL is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// LEVEL is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LEVEL. If not, see <https://www.gnu.org/licenses/>.

#include <VehicleFrequencyManager.hpp>

namespace level::cam {

VehicleFrequencyManager::VehicleFrequencyManager(
    std::shared_ptr<IValueConverter> valueConverter,
    std::shared_ptr<IPOTI> poti)
    : _tGenCAMMin(DEFAULT_T_GEN_CAM_MIN), _tGenCAMMax(DEFAULT_T_GEN_CAM_MAX),
      _tGenCAMLFC(DEFAULT_T_GEN_CAM_LFC),
      _tCheckGenCAM(DEFAULT_T_CHECK_GEN_CAM), _tGenCAM(DEFAULT_T_GEN_CAM_MAX),
      _tGenCAMDCC(DEFAULT_T_GEN_CAM_MIN), _nGenCAMMax(DEFAULT_N_GEN_CAM_MAX),
      _nGenCAM(0), _lastHeading(0), _lastLongitude(0), _lastLatitude(0),
      _lastSpeed(0), _lastTimestamp(0), _lastLFCTimestamp(0),
      _valueConverter(valueConverter), _poti(poti) {}

int VehicleFrequencyManager::getTCheckCAMGen() { return _tCheckGenCAM; }

int VehicleFrequencyManager::getTCAMGen() { return _tGenCAM; }

bool VehicleFrequencyManager::generateCAM(CAMWrapper &cam) {
  auto timeDif = _poti->now() - _lastTimestamp;

  if (timeDif >= (unsigned long long int)_tGenCAMDCC) {

    auto bvhfc = &cam->cam.camParameters.highFrequencyContainer.choice
                      .basicVehicleContainerHighFrequency;
    auto refPos = &cam->cam.camParameters.basicContainer.referencePosition;
    float distance = _valueConverter->distance(
        _lastLongitude, _lastLatitude,
        _valueConverter->itsToSILongitude(refPos->longitude),
        _valueConverter->itsToSILatitude(refPos->latitude));
    float speedDif = abs(
        _lastSpeed - _valueConverter->itsToSISpeed(bvhfc->speed.speedValue));
    float headingDif = abs(_lastHeading - _valueConverter->itsToSIHeading(
                                              bvhfc->heading.headingValue));

    // Trigger by condition 1
    if (headingDif >= 4.0 || speedDif >= 0.5 || distance >= 4.0) {
      _nGenCAM = 1;
      _tGenCAM = timeDif;
      return true;
    }
    // Trigger by condition 2
    else if (timeDif >= (unsigned long long int)_tGenCAM) {
      _nGenCAM++;
      if (_nGenCAM >= _nGenCAMMax) {
        _tGenCAM = _tGenCAMMax;
      }
      return true;
    }
  }
  return false;
}

bool VehicleFrequencyManager::includeLFC() {
  auto timeDif = _poti->now() - _lastLFCTimestamp;
  return timeDif >= (unsigned long long int)_tGenCAMLFC;
}

void VehicleFrequencyManager::notifyCAMGeneration(CAMWrapper &cam) {
  _lastTimestamp = _poti->now();

  auto bvhfc = &cam->cam.camParameters.highFrequencyContainer.choice
                    .basicVehicleContainerHighFrequency;
  auto refPos = &cam->cam.camParameters.basicContainer.referencePosition;

  _lastHeading = _valueConverter->itsToSIHeading(bvhfc->heading.headingValue);
  _lastSpeed = _valueConverter->itsToSISpeed(bvhfc->speed.speedValue);
  _lastLatitude = _valueConverter->itsToSILatitude(refPos->latitude);
  _lastLongitude = _valueConverter->itsToSILongitude(refPos->longitude);

  if (cam->cam.camParameters.lowFrequencyContainer) {
    _lastLFCTimestamp = _lastTimestamp;
  }
}

} // namespace level::cam