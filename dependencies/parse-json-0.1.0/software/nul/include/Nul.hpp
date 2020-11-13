// Nul.hpp
// Nul header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef JSON_NUL_HPP
#define JSON_NUL_HPP

// Standard library
#include <cstddef>   // nullptr_t
#include <ostream>   // ostream
#include <string>    // string

// json library
#include <Type.hpp>  // Type
#include <Value.hpp> // Value

namespace json {
  class Nul: public Value {
  public:
    Nul();
    Nul(std::string& malleableString);
    Nul(const Nul& jsonNul);
    Nul(Nul&& jsonNul);
    virtual ~Nul();
    virtual Value& operator=(const Value& value);
    virtual Nul& operator=(const Nul& jsonNul);
    virtual bool operator==(const Value& value) const;
    virtual bool operator==(const Nul& jsonNul) const;
    virtual bool operator!=(const Value& value) const;
    virtual bool operator!=(const Nul& jsonNul) const;
    virtual Nul* clone() const;
    virtual std::string toString() const;
    virtual Type getType() const;
    virtual bool isValid() const;
    virtual void setInvalid();
    std::nullptr_t getValue() const;
  };
  std::ostream& operator<<(std::ostream& output, const Nul& jsonNul);
}

#endif
