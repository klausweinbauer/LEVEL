#pragma once

namespace level {

class IPOTI {
public:
  virtual ~IPOTI() {}

  /**
   * @brief Current timestamp in milliseconds since 2004-01-01 00:00:00:000 UTC.
   *
   * @return unsigned long long int
   */
  virtual unsigned long long int now() const = 0;
};

} // namespace level