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

#pragma once

#include <CAMWrapper.hpp>
#include <ICABasicService.hpp>
#include <IDatabase.hpp>
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
  CABasicServiceConfig _config;
  bool _disseminationActive;
  std::unique_ptr<std::thread> _disseminationThread;
  std::shared_ptr<IPOTI> _poti;
  std::shared_ptr<IDatabase<CAMWrapper>> _db;

  void disseminationTask();
  void receiveCallback(CAM *msg, bool *tookOwnership);

public:
  virtual ~CABasicService();

  CABasicService(std::shared_ptr<INetworkInterface<CAM>> networkInterface,
                 std::shared_ptr<IValueConverter> valueConverter,
                 std::shared_ptr<IFrequencyManager> frequencyManager,
                 std::shared_ptr<IPOTI> poti,
                 std::shared_ptr<IDatabase<CAMWrapper>> camDatabase);

  virtual void configure(CABasicServiceConfig config) override;

  virtual CABasicServiceConfig getConfiguration() override;

  virtual float getCAMGenerationFrequency() override;

  virtual CAMWrapper cam() override;

  virtual bool tryGetCAM(unsigned int stationID, CAMWrapper *cam) override;

  virtual void setHeading(float heading) override;

  virtual void setSpeed(float speed) override;

  virtual void setDriveDirection(DriveDirectionType direction) override;

  virtual void setAcceleration(float acceleration) override;

  virtual void setCurvature(float radius) override;

  virtual void setYawRate(float yawRate) override;
};

} // namespace level::cam