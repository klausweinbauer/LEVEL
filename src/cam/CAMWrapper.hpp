#pragma once

#include <CAM.h>
#include <CAMServiceProvider.hpp>

namespace level {
namespace cam {

class CAMWrapper {

private:
  CAM_t *_cam;

public:
  CAMWrapper(int stationId);
  CAMWrapper(int stationId, HighFrequencyContainer_PR type);

  ~CAMWrapper();

  CAMWrapper(const CAMWrapper &other);
  CAMWrapper &operator=(CAMWrapper other);

  CAM_t *operator->();

  void setLFC(LowFrequencyContainer_PR type);

  friend void swap(CAMWrapper &first, CAMWrapper &second) {
    using std::swap;
    swap(first._cam, second._cam);
  }
};

} // namespace cam
} // namespace level