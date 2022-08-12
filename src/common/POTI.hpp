#pragma once

#include <IPOTI.hpp>

namespace level {

class POTI : public IPOTI {

public:
  virtual ~POTI();

  virtual unsigned long long int now() const override;
};

} // namespace level