#pragma once

#include <IQRYParameter.hpp>

namespace level {

/**
 * @brief Query to get database elements by a specific parameter value.
 *
 * @tparam T Type of parameter. Must implement a default constructor.
 */
template <typename T> class QRYParameter : public IQRYParameter {
private:
  T _value;

public:
  /**
   * @brief Construct a new parameter value query.
   *
   */
  QRYParameter() : _value(T()) {}

  /**
   * @brief Construct a new parameter value query.
   *
   * @param value The parameter value the query should look for.
   */
  QRYParameter(T value) : _value(value) {}

  /**
   * @brief Parameter value used in this query.
   *
   * @return T Type of parameter.
   */
  T value() const { return _value; }

  /**
   * @brief Set the value for this query.
   *
   * @param value Value to set.
   */
  void setValue(T value) { _value = value; }

  /**
   * @brief Create a query to find entries by a specific parameter value.
   *
   * @param value Value of the parameter.
   * @return std::shared_ptr<QRYParameter<T>> Returns a query.
   */
  static std::shared_ptr<QRYParameter<T>> byValue(T value) {
    return std::make_shared<QRYParameter<T>>(value);
  }
};

} // namespace level