#pragma once

#include <CAMWrapper.hpp>
#include <IFrequencyManager.hpp>
#include <IPOTI.hpp>
#include <IValueConverter.hpp>
#include <memory>

#define DEFAULT_T_GEN_CAM_MIN 100
#define DEFAULT_T_GEN_CAM_MAX 1000
#define DEFAULT_T_GEN_CAM_LFC 500
#define DEFAULT_T_CHECK_GEN_CAM 10
#define DEFAULT_N_GEN_CAM_MAX 3

namespace level::cam {

class VehicleFrequencyManager : public IFrequencyManager {

private:
  int _tGenCAMMin;
  int _tGenCAMMax;
  int _tGenCAMLFC;
  int _tCheckGenCAM;
  int _tGenCAM;
  int _tGenCAMDCC;
  int _nGenCAMMax;
  int _nGenCAM;
  float _lastHeading;
  float _lastLongitude;
  float _lastLatitude;
  float _lastSpeed;
  unsigned long long int _lastTimestamp;
  unsigned long long int _lastLFCTimestamp;
  std::shared_ptr<IValueConverter> _valueConverter;
  std::shared_ptr<IPOTI> _poti;

public:
  VehicleFrequencyManager(std::shared_ptr<IValueConverter> valueConverter,
                          std::shared_ptr<IPOTI> poti);

  virtual ~VehicleFrequencyManager() {}

  virtual int getTCheckCAMGen() override;

  virtual int getTCAMGen() override;

  virtual bool generateCAM(CAMWrapper &cam) override;

  virtual bool includeLFC() override;

  virtual void notifyCAMGeneration(CAMWrapper &cam) override;
};

} // namespace level::cam