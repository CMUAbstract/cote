// Bool.hpp
// Bool header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef JSON_BOOL_HPP
#define JSON_BOOL_HPP

// Standard library
#include <ostream>   // ostream
#include <string>    // string

// json library
#include <Type.hpp>  // Type
#include <Value.hpp> // Value

namespace json {
  class Bool: public Value {
  public:
    Bool();
    Bool(std::string& malleableString);
    Bool(const Bool& jsonBool);
    Bool(Bool&& jsonBool);
    virtual ~Bool();
    virtual Value& operator=(const Value& value);
    virtual Bool& operator=(const Bool& jsonBool);
    virtual bool operator==(const Value& value) const;
    virtual bool operator==(const Bool& jsonBool) const;
    virtual bool operator!=(const Value& value) const;
    virtual bool operator!=(const Bool& jsonBool) const;
    virtual Bool* clone() const;
    virtual std::string toString() const;
    virtual Type getType() const;
    virtual bool isValid() const;
    virtual void setInvalid();
    bool getValue() const;
  private:
    bool boolValue;
  };
  std::ostream& operator<<(std::ostream& output, const Bool& jsonBool);
}

#endif
