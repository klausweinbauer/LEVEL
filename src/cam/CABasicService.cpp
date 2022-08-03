#include <CABasicService.hpp>

namespace level::cam {

CABasicService::CABasicService(std::shared_ptr<IValueConverter> valueConverter)
    : _valueConverter(valueConverter) {}

void CABasicService::configure(CABasicServiceConfig config) {}

CABasicServiceConfig CABasicService::getConfiguration() {}

float CABasicService::getCAMGenerationFrequency() {}

CAMWrapper &CABasicService::cam() { return _cam; }

CAMWrapper CABasicService::getCAM(unsigned int stationID) {}

void CABasicService::setHeading(float heading) {
  if (_cam->cam.camParameters.highFrequencyContainer.present ==
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    int itsValue = _valueConverter->siToITSHeading(heading);
    _cam->cam.camParameters.highFrequencyContainer.choice
        .basicVehicleContainerHighFrequency.heading.headingValue = itsValue;
  }
}

void CABasicService::setSpeed(float speed) {
  if (_cam->cam.camParameters.highFrequencyContainer.present ==
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    int itsValue = _valueConverter->siToITSSpeed(speed);
    _cam->cam.camParameters.highFrequencyContainer.choice
        .basicVehicleContainerHighFrequency.speed.speedValue = itsValue;
  }
}

void CABasicService::setDriveDirection(DriveDirectionType direction) {
  if (_cam->cam.camParameters.highFrequencyContainer.present ==
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    _cam->cam.camParameters.highFrequencyContainer.choice
        .basicVehicleContainerHighFrequency.driveDirection = direction;
  }
}

void CABasicService::setAcceleration(float acceleration) {
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
  if (_cam->cam.camParameters.highFrequencyContainer.present ==
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    int itsValue = _valueConverter->siToITSCurvature(radius);
    _cam->cam.camParameters.highFrequencyContainer.choice
        .basicVehicleContainerHighFrequency.curvature.curvatureValue = itsValue;
  }
}

void CABasicService::setYawRate(float yawRate) {
  if (_cam->cam.camParameters.highFrequencyContainer.present ==
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    int itsValue = _valueConverter->siToITSYawRate(yawRate);
    _cam->cam.camParameters.highFrequencyContainer.choice
        .basicVehicleContainerHighFrequency.yawRate.yawRateValue = itsValue;
  }
}

} // namespace level::cam