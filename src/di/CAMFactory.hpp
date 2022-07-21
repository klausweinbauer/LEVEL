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

  static IDatabase &db() {
    static auto instance = std::unique_ptr<IDatabase>(new InMemoryDatabase());
    return *instance;
  }

  static IEncoder &encoder() {
    static auto instance = std::unique_ptr<IEncoder>(new Encoder());
    return *instance;
  }

  static INetworkInterface &networkAL() {
    // TODO Make port configurable
    static auto instance = std::unique_ptr<INetworkInterface>(
        new SocketBasedNAC(socket(), socket()));
    return *instance;
  }

  static std::shared_ptr<ISocket> socket() {
    return std::shared_ptr<ISocket>(new UDPSocket(5999));
  }
};

} // namespace cam
} // namespace level