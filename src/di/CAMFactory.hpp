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

#include <Encoder.hpp>
#include <InMemoryDatabase.hpp>
#include <SocketBasedNAC.hpp>
#include <UDPSocket.hpp>
#include <level_config.h>

namespace level {
namespace cam {

/**
 * @brief Global instance provider for dependency injection.
 *
 */
class Factory {
private:
  Factory(){};

public:
  ~Factory(){};

  static std::shared_ptr<IDatabase> db() {
    // singelton
    static auto instance = std::shared_ptr<IDatabase>(new InMemoryDatabase());
    return instance;
  }

  static std::shared_ptr<IEncoder> &encoder() {
    // singelton
    static auto instance =
        std::shared_ptr<IEncoder>(new Encoder(level::config::encoding));
    return instance;
  }

  static std::shared_ptr<INetworkInterface> &networkAL() {
    // singelton
    static auto instance = std::shared_ptr<INetworkInterface>(
        new SocketBasedNAC(socket(), socket(), encoder()));
    return instance;
  }

  static std::shared_ptr<ISocket> socket() {
    // transient
    return std::shared_ptr<ISocket>(new UDPSocket(level::config::port));
  }
};

} // namespace cam
} // namespace level