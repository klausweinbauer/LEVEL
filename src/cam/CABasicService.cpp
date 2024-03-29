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

#include <CABasicService.hpp>
#include <CAMIndexer.hpp>
#include <QRYStationID.hpp>

namespace level::cam {

CABasicService::CABasicService(
    std::shared_ptr<INetworkInterface<CAM>> networkInterface,
    std::shared_ptr<IValueConverter> valueConverter,
    std::shared_ptr<IFrequencyManager> frequencyManager,
    std::shared_ptr<IPOTI> poti,
    std::shared_ptr<IDatabase<CAMWrapper>> camDatabase)
    : _nal(networkInterface), _valueConverter(valueConverter),
      _frequencyManager(frequencyManager), _cam(), _disseminationActive(false),
      _poti(poti), _db(camDatabase) {

  // Add indexer to cam database
  _db->addIndexer(std::make_unique<IDXCAMIndex>());
  _db->addIndexer(std::make_unique<IDXCAMStationID>());
  _db->addIndexer(std::make_unique<IDXCAMLatest>());

  _config.stationID = rand();
  _config.stationType = StationType::StationType_PassengerCar;

  configure(_config);

  auto recvHandler = _nal->getRecvHandler();
  auto callbackFunc = [this](CAM *msg, bool *tookOwnership) {
    this->receiveCallback(msg, tookOwnership);
  };
  recvHandler->registerCallback(callbackFunc);

  _disseminationActive = true;
  _disseminationThread = std::unique_ptr<std::thread>(
      new std::thread(([this]() { disseminationTask(); })));
}

void CABasicService::configure(CABasicServiceConfig config) {
  std::lock_guard<std::mutex> guard(_camMutex);
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
  _disseminationThread->join();
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

void CABasicService::receiveCallback(CAM *msg, bool *tookOwnership) {
  _db->insert(std::make_unique<CAMWrapper>(msg));
  // TODO add callback for applications (API callback)
  *tookOwnership = true;
}

CABasicServiceConfig CABasicService::getConfiguration() { return _config; }

float CABasicService::getCAMGenerationFrequency() {
  return 1000.0 / _frequencyManager->getTCAMGen();
}

CAMWrapper CABasicService::cam() {
  std::lock_guard<std::mutex> guard(_camMutex);
  return _cam;
}

bool CABasicService::tryGetCAM(unsigned int stationID, CAMWrapper *cam) {
  auto views = _db->get(QRYLatestMsg::byId(stationID));
  assert(views.size() <= 1 &&
         "There should only be one latest message for each station id.");

  if (views.size() == 0) {
    return false;
  } else {
    *cam = *views.at(0);
    return true;
  }
}

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