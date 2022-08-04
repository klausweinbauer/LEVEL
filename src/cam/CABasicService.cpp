#include <CABasicService.hpp>

namespace level::cam {

CABasicService::CABasicService(
    std::shared_ptr<INetworkInterface<CAM>> networkInterface,
    std::shared_ptr<IValueConverter> valueConverter,
    std::shared_ptr<IFrequencyManager> frequencyManager,
    std::shared_ptr<IPOTI> poti)
    : _nal(networkInterface), _valueConverter(valueConverter),
      _frequencyManager(frequencyManager), _disseminationActive(true),
      _disseminationThread([this]() { disseminationTask(); }), _poti(poti) {

  configure(_config);
}

void CABasicService::configure(CABasicServiceConfig config) {
  _config = config;
  _cam->header.stationID = _config.stationID;
  _cam->cam.camParameters.basicContainer.stationType = _config.stationType;
  if (_config.stationType == StationType_RoadSideUnit) {
    _cam.setHFC(HighFrequencyContainer_PR_rsuContainerHighFrequency);
    _cam.clearLFC();
  } else if (_config.stationType == StationType_Unknown) {
    _cam.setHFC(HighFrequencyContainer_PR_NOTHING);
    _cam.clearLFC();
  } else {
    _cam.setHFC(HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
    _cam.setLFC(LowFrequencyContainer_PR_basicVehicleContainerLowFrequency);
  }
}

CABasicService::~CABasicService() {
  _disseminationActive = false;
  _disseminationThread.join();
}

void CABasicService::disseminationTask() {
  while (_disseminationActive) {
    int sleepTime = _frequencyManager->getTCheckCAMGen();
    assert(sleepTime <= 1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

    _camMutex.lock();
    if (_frequencyManager->generateCAM(_cam)) {
      CAMWrapper genCAM = _cam;
      _camMutex.unlock();
      genCAM->cam.generationDeltaTime =
          _valueConverter->timestampToDeltaTime(_poti->now());
      if (!_frequencyManager->includeLFC()) {
        genCAM.clearLFC();
      }
      _nal->send(genCAM.get());
      _frequencyManager->notifyCAMGeneration(genCAM);
    } else {
      _camMutex.unlock();
    }
  }
}

CABasicServiceConfig CABasicService::getConfiguration() { return _config; }

float CABasicService::getCAMGenerationFrequency() {}

CAMWrapper CABasicService::cam() {
  std::lock_guard<std::mutex> guard(_camMutex);
  return _cam;
}

CAMWrapper CABasicService::getCAM(unsigned int stationID) {}

void CABasicService::setHeading(float heading) {
  std::lock_guard<std::mutex> guard(_camMutex);
  if (_cam->cam.camParameters.highFrequencyContainer.present ==
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    int itsValue = _valueConverter->siToITSHeading(heading);
    _cam->cam.camParameters.highFrequencyContainer.choice
        .basicVehicleContainerHighFrequency.heading.headingValue = itsValue;
  }
}

void CABasicService::setSpeed(float speed) {
  std::lock_guard<std::mutex> guard(_camMutex);
  if (_cam->cam.camParameters.highFrequencyContainer.present ==
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    int itsValue = _valueConverter->siToITSSpeed(speed);
    _cam->cam.camParameters.highFrequencyContainer.choice
        .basicVehicleContainerHighFrequency.speed.speedValue = itsValue;
  }
}

void CABasicService::setDriveDirection(DriveDirectionType direction) {
  std::lock_guard<std::mutex> guard(_camMutex);
  if (_cam->cam.camParameters.highFrequencyContainer.present ==
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    _cam->cam.camParameters.highFrequencyContainer.choice
        .basicVehicleContainerHighFrequency.driveDirection = direction;
  }
}

void CABasicService::setAcceleration(float acceleration) {
  std::lock_guard<std::mutex> guard(_camMutex);
  if (_cam->cam.camParameters.highFrequencyContainer.present ==
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    int itsValue =
        _valueConverter->siToITSLongitudinalAcceleration(acceleration);
    _cam->cam.camParameters.highFrequencyContainer.choice
        .basicVehicleContainerHighFrequency.longitudinalAcceleration
        .longitudinalAccelerationValue = itsValue;
  }
}

void CABasicService::setCurvature(float radius) {
  std::lock_guard<std::mutex> guard(_camMutex);
  if (_cam->cam.camParameters.highFrequencyContainer.present ==
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    int itsValue = _valueConverter->siToITSCurvature(radius);
    _cam->cam.camParameters.highFrequencyContainer.choice
        .basicVehicleContainerHighFrequency.curvature.curvatureValue = itsValue;
  }
}

void CABasicService::setYawRate(float yawRate) {
  std::lock_guard<std::mutex> guard(_camMutex);
  if (_cam->cam.camParameters.highFrequencyContainer.present ==
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    int itsValue = _valueConverter->siToITSYawRate(yawRate);
    _cam->cam.camParameters.highFrequencyContainer.choice
        .basicVehicleContainerHighFrequency.yawRate.yawRateValue = itsValue;
  }
}

} // namespace level::cam