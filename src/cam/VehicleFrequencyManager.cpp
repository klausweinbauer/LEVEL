#include <VehicleFrequencyManager.hpp>

namespace level::cam {

VehicleFrequencyManager::VehicleFrequencyManager()
    : _tGenCAMMin(DEFAULT_T_GEN_CAM_MIN), _tGenCAMMax(DEFAULT_T_GEN_CAM_MAX),
      _tCheckGenCAM(DEFAULT_T_CHECK_GEN_CAM), _tGenCAM(DEFAULT_T_GEN_CAM_MAX),
      _nGenCAMMax(DEFAULT_N_GEN_CAM_MAX), _nGenCAM(0) {}

int VehicleFrequencyManager::getTCheckCAMGen() { return _tCheckGenCAM; }

bool VehicleFrequencyManager::generateCAM(const CAMWrapper &cam) {}

bool VehicleFrequencyManager::includeLFC() {}

void VehicleFrequencyManager::notifyCAMGeneration(const CAMWrapper &cam) {}

} // namespace level::cam