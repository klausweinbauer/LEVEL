#pragma once

#include <DBView.hpp>
#include <memory>

namespace c2x::cam {

class IDatabase {
private:
public:
  virtual ~IDatabase(){};

  virtual DBView<int, double> getById(int id) = 0;
  virtual DBView<int, double> create(int id) = 0;
};
}; // namespace c2x::cam