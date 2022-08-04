#pragma once

#include <CAMWrapper.hpp>
#include <IFrequencyManager.hpp>
#include <IPOTI.hpp>
#include <memory>

#define DEFAULT_T_GEN_CAM_MIN 100
#define DEFAULT_T_GEN_CAM_MAX 1000
#define DEFAULT_T_CHECK_GEN_CAM 10
#define DEFAULT_N_GEN_CAM_MAX 3

namespace level::cam {

class VehicleFrequencyManager : public IFrequencyManager {

private:
  int _tGenCAMMin;
  int _tGenCAMMax;
  int _tCheckGenCAM;
  int _tGenCAM;
  int _nGenCAMMax;
  int _nGenCAM;
  float _lastHeading;
  float _lastLongitude;
  float _lastLatitude;
  float _lastSpeed;
  unsigned long long int _lastTimestamp;

public:
  VehicleFrequencyManager();

  virtual ~VehicleFrequencyManager() {}

  virtual int getTCheckCAMGen() override;

  virtual bool generateCAM(const CAMWrapper &cam) override;

  virtual bool includeLFC() override;

  virtual void notifyCAMGeneration(const CAMWrapper &cam) override;
};

} // namespace level::cam