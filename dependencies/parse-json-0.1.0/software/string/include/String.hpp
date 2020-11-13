// String.hpp
// String header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef JSON_STRING_HPP
#define JSON_STRING_HPP

// Standard library
#include <ostream>   // ostream
#include <string>    // string

// json library
#include <Type.hpp>  // Type
#include <Value.hpp> // Value

namespace json {
  class String: public Value {
  public:
    String();
    String(std::string& malleableString);
    String(const String& jsonString);
    String(String&& jsonString);
    virtual ~String();
    virtual Value& operator=(const Value& value);
    virtual String& operator=(const String& jsonString);
    virtual bool operator==(const Value& value) const;
    virtual bool operator==(const String& jsonString) const;
    virtual bool operator!=(const Value& value) const;
    virtual bool operator!=(const String& jsonString) const;
    virtual String* clone() const;
    virtual std::string toString() const;
    virtual Type getType() const;
    virtual bool isValid() const;
    virtual void setInvalid();
    std::string getValue() const;
  private:
    std::string stringValue;
  };
  std::ostream& operator<<(std::ostream& output, const String& jsonString);
}

#endif
