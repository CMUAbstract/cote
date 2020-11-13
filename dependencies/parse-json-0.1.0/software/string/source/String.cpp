// String.cpp
// String implementation file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <ostream>       // ostream
#include <sstream>       // ostringstream
#include <string>        // string

// json library
#include <String.hpp>    // String
#include <Type.hpp>      // Type
#include <Value.hpp>     // Value
#include <json-util.hpp> // parseString

namespace json {
  String::String() : Value() {
    this->setInvalid();
  }

  String::String(std::string& malleableString) : Value() {
    if(malleableString.substr(0,1)=="\"") {
      this->stringValue = util::parseString(malleableString);
    } else {
      this->setInvalid();
    }
  }

  String::String(const String& jsonString) : Value(jsonString),
   stringValue(jsonString.getValue()) {}

  String::String(String&& jsonString) : Value(jsonString),
   stringValue(jsonString.stringValue) {}

  String::~String() {}

  Value& String::operator=(const Value& value) {
    if(this!=&value && this->getType()==value.getType()) {
      if(!value.isValid()) {
        this->setInvalid();
      }
      this->stringValue = dynamic_cast<const String&>(value).getValue();
    } else if(this->getType()!=value.getType()) {
      this->setInvalid();
    }
    return *this;
  }

  String& String::operator=(const String& jsonString) {
    if(this!=&jsonString) {
      if(!jsonString.isValid()) {
        this->setInvalid();
      }
      this->stringValue = jsonString.getValue();
    }
    return *this;
  }

  bool String::operator==(const Value& value) const {
    return (this->isValid()==value.isValid()) &&
           (this->getType()==value.getType()) &&
           (this->stringValue==dynamic_cast<const String&>(value).getValue());
  }

  bool String::operator==(const String& jsonString) const {
    return (this->isValid()==jsonString.isValid()) &&
           (this->stringValue==jsonString.getValue());
  }

  bool String::operator!=(const Value& value) const {
    return !(this->operator==(value));
  }

  bool String::operator!=(const String& jsonString) const {
    return !(this->operator==(jsonString));
  }

  String* String::clone() const {
    return new String(*this);
  }

  std::string String::toString() const {
    std::ostringstream oss;
    if(!this->isValid()) {
      oss << "JSON STRING INVALID";
    } else {
      oss << "\"" << this->stringValue << "\"";
    }
    return oss.str();
  }

  Type String::getType() const {
    return Type::STRING;
  }

  bool String::isValid() const {
    return this->Value::isValid();
  }

  void String::setInvalid() {
    this->Value::setInvalid();
  }

  std::string String::getValue() const {
    return this->stringValue;
  }

  std::ostream& operator<<(std::ostream& output, const String& jsonString) {
    output << jsonString.toString();
    return output;
  }
}
