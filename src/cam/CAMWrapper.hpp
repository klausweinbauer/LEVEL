#pragma once

#include <CAM.h>
#include <Types.hpp>
#include <algorithm>

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

#define CAM_TYPE_NAME "CAM"

template <> struct TypeName<CAM> {
  static const char *get() { return CAM_TYPE_NAME; }
};

template <> struct TypeName<cam::CAMWrapper> {
  static const char *get() { return CAM_TYPE_NAME; }
};

} // namespace level