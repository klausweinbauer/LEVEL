
#include <CAM.h>

namespace c2x::cam {

class CAM {
private:
  CAM_t *_cam;

public:
  CAM(int stationID);
  ~CAM();

  CAM(const CAM &other) = delete;
  CAM &operator=(const CAM &other) = delete;

  CAM(CAM &&cam);
  CAM &operator=(CAM &&cam);

  CAM_t *operator->();
};

} // namespace c2x::cam
