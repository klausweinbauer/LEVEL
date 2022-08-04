#pragma once

#include <CABasicService.hpp>
#include <UDPSocket.hpp>
#include <functional>
#include <memory>

#define INSTANTIATION_FUNCTION(interface, implementation, parameters)          \
  inline std::function<std::shared_ptr<interface>()> di##interface##Provider = \
      []() { return std::make_shared<implementation>(parameters); };

#define REG_TRANSIENT(interface, implementation, parameters)                   \
  INSTANTIATION_FUNCTION(interface, implementation, parameters)                \
  inline std::shared_ptr<interface> get##interface() {                         \
    return di##interface##Provider();                                          \
  }

#define REG_SINGELTON(interface, implementation, parameters)                   \
  INSTANTIATION_FUNCTION(interface, implementation, parameters)                \
  inline std::shared_ptr<interface> get##interface() {                         \
    static std::shared_ptr<interface> instance = di##interface##Provider();    \
    return instance;                                                           \
  }

namespace level::di {

using namespace level::cam;

REG_SINGELTON(IValueConverter, ValueConverter, )
REG_SINGELTON(VDP_t, VDP_t, )
REG_SINGELTON(CABasicService, CABasicService, getIValueConverter())

} // namespace level::di