// Array.hpp
// Array header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef JSON_ARRAY_HPP
#define JSON_ARRAY_HPP

// Standard library
#include <ostream>   // ostream
#include <string>    // string
#include <vector>    // vector

// json library
#include <Type.hpp>  // Type
#include <Value.hpp> // Value

namespace json {
  class Array: public Value {
  public:
    Array();
    Array(std::string& malleableString);
    Array(const Array& jsonArray);
    Array(Array&& jsonArray);
    virtual ~Array();
    virtual Value& operator=(const Value& value);
    virtual Array& operator=(const Array& jsonArray);
    virtual bool operator==(const Value& value) const;
    virtual bool operator==(const Array& jsonArray) const;
    virtual bool operator!=(const Value& value) const;
    virtual bool operator!=(const Array& jsonArray) const;
    virtual Array* clone() const;
    virtual std::string toString() const;
    virtual Type getType() const;
    virtual bool isValid() const;
    virtual void setInvalid();
    std::vector<Value*> getValue() const;
  private:
    std::vector<Value*> arrayValue;
  };
  std::ostream& operator<<(std::ostream& output, const Array& jsonArray);
}

#endif
