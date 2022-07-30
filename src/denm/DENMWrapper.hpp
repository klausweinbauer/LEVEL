#pragma once

#include <DENM.h>
#include <DENMEncoder.hpp>
#include <Types.hpp>
#include <algorithm>

namespace level {
namespace denm {

/**
 * @brief Memory safe wrapper for DENMs.
 *
 */
class DENMWrapper {

private:
  DENM *_denm;
  static DEREncoder _copyEncoder;

public:
  DENMWrapper();
  DENMWrapper(int stationId, int sequenceNumber);

  ~DENMWrapper();

  DENMWrapper(const DENMWrapper &other);
  DENMWrapper &operator=(DENMWrapper other);

  DENM *operator->();
  DENM &operator*();

  friend void swap(DENMWrapper &first, DENMWrapper &second) {
    using std::swap;
    swap(first._denm, second._denm);
  }
};

} // namespace denm

#define DENM_TYPE_NAME "DENM"

template <> struct TypeName<DENM> {
  static const char *get() { return DENM_TYPE_NAME; }
};

template <> struct TypeName<denm::DENMWrapper> {
  static const char *get() { return DENM_TYPE_NAME; }
};

} // namespace level