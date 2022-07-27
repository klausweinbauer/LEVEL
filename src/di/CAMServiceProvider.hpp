/**
 * @file CAMServiceProvider.hpp
 * @author Klaus Weinbauer
 * @brief Dependency injection provider for CAM namespace.
 * @version 0.1
 * @date 2022-07-22
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <Encoder.hpp>
#include <SocketBasedNI.hpp>
#include <Syscall.hpp>
#include <UDPSocket.hpp>
#include <functional>
#include <level_config.h>

namespace level {
namespace cam {
namespace di {

static std::function<std::shared_ptr<ISocket>()> socketProvider = []() {
  // transient
  return std::shared_ptr<ISocket>(
      new UDPSocket(level::config::port, std::make_shared<Syscall>()));
};
static std::shared_ptr<ISocket> socket() { return socketProvider(); }

static std::function<std::shared_ptr<IEncoder>()> encoderProvider = []() {
  // singelton
  static auto instance =
      std::shared_ptr<IEncoder>(new Encoder(level::config::encoding));
  return instance;
};

static std::shared_ptr<IEncoder> encoder() { return encoderProvider(); }

static std::function<std::shared_ptr<INetworkInterface>()> networkProvider =
    []() {
      // singelton
      static auto instance = std::shared_ptr<INetworkInterface>(
          new SocketBasedNI(socket(), socket(), encoder()));
      return instance;
    };
static std::shared_ptr<INetworkInterface> network() {
  return networkProvider();
}

} // namespace di
} // namespace cam
} // namespace level