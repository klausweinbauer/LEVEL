/**
 * @file CAMData.hpp
 * @author Klaus Weinbauer
 * @brief Stores CAM with some meta data needed by the service.
 * @version 0.1
 * @date 2022-07-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <CAM.h>

namespace level {
  namespace cam {

struct CAMMetadata {
  long _pHeadingValue;
  long _pLatitude;
  long _pLongitude;
  long _pGenDeltaTime;
};

/**
 * @brief Stores CAM with some meta data needed by the service.
 *
 * @details This class manages a single instance of CAM with additional data
 * required by the Cooperative Awareness basic service.
 */
class CAMData {
private:
  CAM_t *_cam;
  CAMMetadata _mData;

public:
  CAMData(CAM_t *cam) : _cam(cam) {}
  virtual ~CAMData() { ASN_STRUCT_FREE(asn_DEF_CAM, _cam); }

  CAMData(const CAMData &other) = delete;
  CAMData &operator=(const CAMData &other) = delete;

  CAMData(CAMData &&other) {
    _cam = other._cam;
    _mData = other._mData;
    other._cam = nullptr;
    other._mData = CAMMetadata();
  }

  CAMData &operator=(CAMData &&other) {
    if (this != &other) {
      _cam = other._cam;
      _mData = other._mData;
      other._cam = nullptr;
      other._mData = CAMMetadata();
    }
    return *this;
  }

  CAM_t *cam() { return _cam; }

  CAMMetadata data() { return _mData; }
};

}
} // namespace level::cam
