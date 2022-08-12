/**
 * @file RecvHandler.hpp
 * @author Klaus Weinbauer
 * @brief Implementation for IRecvHandler interface.
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <IRecvHandler.hpp>
#include <vector>

namespace level {

/**
 * @brief Implementation for IRecvHandler interface.
 *
 * @tparam T Message type.
 */
template <typename T> class RecvHandler : public IRecvHandler<T> {
protected:
  std::vector<std::function<void(T *msg, bool *tookOwnership)>> _callbacks;

  virtual void freeMessage(T *msg) = 0;

public:
  RecvHandler() : _callbacks() {}

  virtual ~RecvHandler() {}

  virtual void registerCallback(
      std::function<void(T *msg, bool *tookOwnership)> callback) override {
    _callbacks.push_back(callback);
  }

  virtual void unregisterCallbacks() override { _callbacks.clear(); }

  virtual int callbackCount() override { return _callbacks.size(); }

  virtual void invoke(T *msg) override {
    bool tookOwnership = false;
    for (auto &&callback : _callbacks) {
      callback(msg, &tookOwnership);
    }
    if (!tookOwnership) {
      this->freeMessage(msg);
    }
  }
};

} // namespace level