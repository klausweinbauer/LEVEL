#include <POTI.hpp>
#include <chrono>

namespace level {

POTI::~POTI() {}

unsigned long long int POTI::now() const {
  // Offset from Unix Timestamp to 2004-01-01 00:00:00:000 UTC in ms.
  unsigned long long int offset = 1072915200000;
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
             .count() -
         offset;
}

} // namespace level