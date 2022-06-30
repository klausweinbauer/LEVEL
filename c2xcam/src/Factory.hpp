/**
 * @file Factory.hpp
 * @author Klaus Weinbauer
 * @brief Global instance provider for dependency injection.
 * @version 0.1
 * @date 2022-06-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <InMemoryDatabase.hpp>

namespace c2x::cam {

class Factory {
private:
  Factory(){};

public:
  ~Factory(){};

  static IDatabase &database() {
    static auto instance =
        std::unique_ptr<InMemoryDatabase>(new InMemoryDatabase());
    return *instance;
  }
};

}; // namespace c2x::cam