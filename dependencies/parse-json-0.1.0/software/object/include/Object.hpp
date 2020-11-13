// Object.hpp
// Object header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef JSON_OBJECT_HPP
#define JSON_OBJECT_HPP

// Standard library
#include <map>       // map
#include <ostream>   // ostream
#include <string>    // string

// json library
#include <Type.hpp>  // Type
#include <Value.hpp> // Value

namespace json {
  class Object: public Value {
  public:
    Object();
    Object(std::string& malleableString);
    Object(const Object& jsonObject);
    Object(Object&& jsonObject);
    virtual ~Object();
    virtual Value& operator=(const Value& value);
    virtual Object& operator=(const Object& jsonObject);
    virtual bool operator==(const Value& value) const;
    virtual bool operator==(const Object& jsonObject) const;
    virtual bool operator!=(const Value& value) const;
    virtual bool operator!=(const Object& jsonObject) const;
    virtual Object* clone() const;
    virtual std::string toString() const;
    virtual Type getType() const;
    virtual bool isValid() const;
    virtual void setInvalid();
    std::map<std::string,Value*> getValue() const;
  private:
    std::map<std::string,Value*> objectValue;
  };
  std::ostream& operator<<(std::ostream& output, const Object& jsonObject);
}

#endif
