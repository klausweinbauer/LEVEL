#pragma once

#include <IErrorHandler.hpp>

namespace level {

class ErrorHandler : public IErrorHandler {
protected:
  std::vector<std::function<void(const Exception &)>> _callbacks;

public:
  virtual ~ErrorHandler() {}

  virtual void
  registerCallback(std::function<void(const Exception &)> callback) override {
    _callbacks.push_back(callback);
  }

  virtual void unregisterCallbacks() override { _callbacks.clear(); }

  virtual int callbackCount() override { return _callbacks.size(); }

  virtual void invoke(const Exception &exception) override {
    for (auto &&callback : _callbacks) {
      callback(exception);
    }
  }
};

} // namespace level