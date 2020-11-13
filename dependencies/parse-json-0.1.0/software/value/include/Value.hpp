// Value.hpp
// Value interface class header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef JSON_VALUE_HPP
#define JSON_VALUE_HPP

// Standard library
#include <ostream>  // ostream
#include <string>   // string

// json library
#include <Type.hpp> // Type

namespace json {
  class Value {
  public:
    Value();
    Value(const Value& value);
    Value(Value&& value);
    virtual ~Value();
    virtual Value& operator=(const Value& value) = 0;
    virtual bool operator==(const Value& value) const = 0;
    virtual bool operator!=(const Value& value) const = 0;
    virtual Value* clone() const = 0;
    virtual std::string toString() const = 0;
    virtual Type getType() const = 0;
    virtual bool isValid() const;
    virtual void setInvalid();
  private:
    bool valid;
  };
  std::ostream& operator<<(std::ostream& output, const Value& value);
}

#endif
