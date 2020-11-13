// Number.hpp
// Number header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef JSON_NUMBER_HPP
#define JSON_NUMBER_HPP

// Standard library
#include <ostream>   // ostream
#include <string>    // string

// json library
#include <Type.hpp>  // Type
#include <Value.hpp> // Value

namespace json {
  class Number: public Value {
  public:
    Number();
    Number(std::string& malleableString);
    Number(const Number& jsonNumber);
    Number(Number&& jsonNumber);
    virtual ~Number();
    virtual Value& operator=(const Value& value);
    virtual Number& operator=(const Number& jsonNumber);
    virtual bool operator==(const Value& value) const;
    virtual bool operator==(const Number& jsonNumber) const;
    virtual bool operator!=(const Value& value) const;
    virtual bool operator!=(const Number& jsonNumber) const;
    virtual Number* clone() const;
    virtual std::string toString() const;
    virtual Type getType() const;
    virtual bool isValid() const;
    virtual void setInvalid();
    double getValue() const;
  private:
    double numberValue;
  };
  std::ostream& operator<<(std::ostream& output, const Number& jsonNumber);
}

#endif
