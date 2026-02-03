#ifndef YATEST_EXPECT_H_
#define YATEST_EXPECT_H_

#include <stdexcept>
#include <sstream>
#include <string>
#include <type_traits>

namespace yatest {

struct ExpectationFailed final : public std::logic_error {
  using logic_error::logic_error;
};

// Original expect function for backward compatibility
inline void expect(bool expectation, const char* what) { 
  if (!expectation) throw ExpectationFailed(what); 
}

// New overload with optional message
inline void expect(bool expectation) { 
  if (!expectation) {
    throw ExpectationFailed("assertion failed");
  }
}

namespace expect {

// Template helpers for comparison operations with automatic error messages
namespace detail {
  // SFINAE helper to detect if a type supports operator<<
  template<typename T, typename = void>
  struct has_ostream_operator : std::false_type {};
  
  template<typename T>
  struct has_ostream_operator<T, 
    decltype(void(std::declval<std::ostream&>() << std::declval<const T&>()))
  > : std::true_type {};

  // Convert value to string for types that support operator<<
  template<typename T>
  typename std::enable_if<has_ostream_operator<T>::value, std::string>::type
  toString(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
  }
  
  // Fallback for types that don't support operator<<
  template<typename T>
  typename std::enable_if<!has_ostream_operator<T>::value, std::string>::type
  toString(const T&) {
    return "<value>";
  }
  
  // Specialization for bool
  template<>
  inline std::string toString<bool>(const bool& value) {
    return value ? "true" : "false";
  }
  
  // Specialization for char
  template<>
  inline std::string toString<char>(const char& value) {
    std::ostringstream oss;
    oss << "'" << value << "'";
    return oss.str();
  }
}

// Generic expect function with optional message (alias for yatest::expect)
inline void that(bool expectation, const char* what = "") { 
  if (!expectation) {
    if (what && *what != '\0') {
      throw ExpectationFailed(what);
    } else {
      throw ExpectationFailed("assertion failed");
    }
  }
}

// equals: Assert that actual == expected
template<typename T, typename U>
inline void equals(const T& actual, const U& expected, const char* message = "") {
  if (!(actual == expected)) {
    std::string msg;
    // Try to build a descriptive message if both types support operator<<
    if (detail::has_ostream_operator<T>::value && detail::has_ostream_operator<U>::value) {
      msg = "Expected " + detail::toString(expected) + " but got " + detail::toString(actual);
    } else {
      msg = "Expected values to be equal";
    }
    if (message && *message != '\0') {
      msg += " (" + std::string(message) + ")";
    }
    throw ExpectationFailed(msg.c_str());
  }
}

// notEquals: Assert that actual != expected
template<typename T, typename U>
inline void notEquals(const T& actual, const U& expected, const char* message = "") {
  if (actual == expected) {
    std::string msg;
    if (detail::has_ostream_operator<U>::value) {
      msg = "Expected value to not equal " + detail::toString(expected);
    } else {
      msg = "Expected values to not be equal";
    }
    if (message && *message != '\0') {
      msg += " (" + std::string(message) + ")";
    }
    throw ExpectationFailed(msg.c_str());
  }
}

// isTrue: Assert that value is true
inline void isTrue(bool value, const char* message = "") {
  if (!value) {
    std::string msg = "Expected true";
    if (message && *message != '\0') {
      msg += " (" + std::string(message) + ")";
    }
    throw ExpectationFailed(msg.c_str());
  }
}

// isFalse: Assert that value is false
inline void isFalse(bool value, const char* message = "") {
  if (value) {
    std::string msg = "Expected false";
    if (message && *message != '\0') {
      msg += " (" + std::string(message) + ")";
    }
    throw ExpectationFailed(msg.c_str());
  }
}

// isNull: Assert that pointer is null
template<typename T>
inline void isNull(const T* value, const char* message = "") {
  if (value != nullptr) {
    std::string msg = "Expected null pointer";
    if (message && *message != '\0') {
      msg += " (" + std::string(message) + ")";
    }
    throw ExpectationFailed(msg.c_str());
  }
}

// isNotNull: Assert that pointer is not null
template<typename T>
inline void isNotNull(const T* value, const char* message = "") {
  if (value == nullptr) {
    std::string msg = "Expected non-null pointer";
    if (message && *message != '\0') {
      msg += " (" + std::string(message) + ")";
    }
    throw ExpectationFailed(msg.c_str());
  }
}

// lessThan: Assert that actual < expected
template<typename T>
inline void lessThan(const T& actual, const T& expected, const char* message = "") {
  if (!(actual < expected)) {
    std::string msg;
    if (detail::has_ostream_operator<T>::value) {
      msg = "Expected " + detail::toString(actual) + " < " + detail::toString(expected);
    } else {
      msg = "Expected actual < expected";
    }
    if (message && *message != '\0') {
      msg += " (" + std::string(message) + ")";
    }
    throw ExpectationFailed(msg.c_str());
  }
}

// lessEqual: Assert that actual <= expected
template<typename T>
inline void lessEqual(const T& actual, const T& expected, const char* message = "") {
  if (!(actual <= expected)) {
    std::string msg;
    if (detail::has_ostream_operator<T>::value) {
      msg = "Expected " + detail::toString(actual) + " <= " + detail::toString(expected);
    } else {
      msg = "Expected actual <= expected";
    }
    if (message && *message != '\0') {
      msg += " (" + std::string(message) + ")";
    }
    throw ExpectationFailed(msg.c_str());
  }
}

// greaterThan: Assert that actual > expected
template<typename T>
inline void greaterThan(const T& actual, const T& expected, const char* message = "") {
  if (!(actual > expected)) {
    std::string msg;
    if (detail::has_ostream_operator<T>::value) {
      msg = "Expected " + detail::toString(actual) + " > " + detail::toString(expected);
    } else {
      msg = "Expected actual > expected";
    }
    if (message && *message != '\0') {
      msg += " (" + std::string(message) + ")";
    }
    throw ExpectationFailed(msg.c_str());
  }
}

// greaterEqual: Assert that actual >= expected
template<typename T>
inline void greaterEqual(const T& actual, const T& expected, const char* message = "") {
  if (!(actual >= expected)) {
    std::string msg;
    if (detail::has_ostream_operator<T>::value) {
      msg = "Expected " + detail::toString(actual) + " >= " + detail::toString(expected);
    } else {
      msg = "Expected actual >= expected";
    }
    if (message && *message != '\0') {
      msg += " (" + std::string(message) + ")";
    }
    throw ExpectationFailed(msg.c_str());
  }
}

}

}

#endif
