#pragma once

#pragma once

#include <CAM.h>
#include <DBView.hpp>
#include <memory>

namespace c2x::cam {

class IDatabase {
private:
public:
  virtual ~IDatabase(){};

  virtual bool exists(long unsigned int id) = 0;
  virtual DBView<CAM_t> get(long unsigned int id) = 0;
  virtual DBView<CAM_t> create(long unsigned int id) = 0;
  virtual void remove(long unsigned int id) = 0;
  virtual size_t entryCount() = 0;
};
}; // namespace c2x::cam