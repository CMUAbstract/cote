// Value.cpp
// Value interface class implementation
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <ostream>   // ostream

// json library
#include <Value.hpp> // Value

namespace json {
  Value::Value() : valid(true) {}

  Value::Value(const Value& value) : valid(value.isValid()) {}

  Value::Value(Value&& value) : valid(value.valid) {}

  Value::~Value() {}

  bool Value::isValid() const {
    return this->valid;
  }

  void Value::setInvalid() {
    this->valid = false;
  }

  std::ostream& operator<<(std::ostream& output, const Value& value) {
    output << value.toString();
    return output;
  }
}
