#pragma once

#include <CABasicService.hpp>
#include <memory>

#define REG_SINGELTON(interface, implementation, parameters)                   \
  std::shared_ptr<interface> get##interface() {                                \
    return std::make_shared<implementation>(parameters);                       \
  }

#define REG_TRANSIENT(interface, implementation, parameters)                   \
  std::shared_ptr<interface> get##interface() {                                \
    static std::shared_ptr<interface> instance =                               \
        std::make_shared<implementation>(parameters);                          \
    return instance;                                                           \
  }

namespace level::di {

REG_SINGELTON(CABasicService, CABasicService, )

} // namespace level::di