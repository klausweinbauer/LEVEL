#pragma once

#include <CAMWrapper.hpp>
#include <ICABasicService.hpp>
#include <IFrequencyManager.hpp>
#include <INetworkInterface.hpp>
#include <IPOTI.hpp>
#include <ValueConverter.hpp>
#include <functional>
#include <level.h>
#include <mutex>
#include <thread>

namespace level::cam {

class CABasicService : public ICABasicService {
private:
  std::shared_ptr<INetworkInterface<CAM>> _nal;
  std::shared_ptr<IValueConverter> _valueConverter;
  std::shared_ptr<IFrequencyManager> _frequencyManager;
  CAMWrapper _cam;
  std::mutex _camMutex;
  CABasicServiceConfig _config{.stationID = 0,
                               .stationType = StationType_Unknown};
  bool _disseminationActive;
  std::unique_ptr<std::thread> _disseminationThread;
  std::shared_ptr<IPOTI> _poti;

  void disseminationTask();

public:
  virtual ~CABasicService();

  CABasicService(std::shared_ptr<INetworkInterface<CAM>> networkInterface,
                 std::shared_ptr<IValueConverter> valueConverter,
                 std::shared_ptr<IFrequencyManager> frequencyManager,
                 std::shared_ptr<IPOTI> poti);

  virtual void configure(CABasicServiceConfig config) override;

  virtual CABasicServiceConfig getConfiguration() override;

  virtual float getCAMGenerationFrequency() override;

  virtual CAMWrapper cam() override;

  virtual void setHeading(float heading) override;

  virtual void setSpeed(float speed) override;

  virtual void setDriveDirection(DriveDirectionType direction) override;

  virtual void setAcceleration(float acceleration) override;

  virtual void setCurvature(float radius) override;

  virtual void setYawRate(float yawRate) override;
};

} // namespace level::cam