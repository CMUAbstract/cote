// Bool.cpp
// Bool implementation file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <ostream>   // ostream
#include <sstream>   // ostringstream
#include <string>    // string

// json library
#include <Bool.hpp>  // Bool
#include <Type.hpp>  // Type
#include <Value.hpp> // Value

namespace json {
  Bool::Bool() : Value() {
    this->setInvalid();
  }

  Bool::Bool(std::string& malleableString) : Value() {
    if(malleableString.substr(0,4)=="true") {
      this->boolValue = true;
      malleableString.erase(0,4);
    } else if(malleableString.substr(0,5)=="false") {
      this->boolValue = false;
      malleableString.erase(0,5);
    } else {
      this->setInvalid();
    }
  }

  Bool::Bool(const Bool& jsonBool) : Value(jsonBool),
   boolValue(jsonBool.getValue()) {}

  Bool::Bool(Bool&& jsonBool) : Value(jsonBool),
   boolValue(jsonBool.boolValue) {}

  Bool::~Bool() {}

  Value& Bool::operator=(const Value& value) {
    if(this!=&value && this->getType()==value.getType()) {
      if(!value.isValid()) {
        this->setInvalid();
      }
      this->boolValue = dynamic_cast<const Bool&>(value).getValue();
    } else if(this->getType()!=value.getType()) {
      this->setInvalid();
    }
    return *this;
  }

  Bool& Bool::operator=(const Bool& jsonBool) {
    if(this!=&jsonBool) {
      if(!jsonBool.isValid()) {
        this->setInvalid();
      }
      this->boolValue = jsonBool.getValue();
    }
    return *this;
  }

  bool Bool::operator==(const Value& value) const {
    return (this->isValid()==value.isValid()) &&
           (this->getType()==value.getType()) &&
           (this->boolValue==dynamic_cast<const Bool&>(value).getValue());
  }

  bool Bool::operator==(const Bool& jsonBool) const {
    return (this->isValid()==jsonBool.isValid()) &&
           (this->boolValue==jsonBool.getValue());
  }

  bool Bool::operator!=(const Value& value) const {
    return !(this->operator==(value));
  }

  bool Bool::operator!=(const Bool& jsonBool) const {
    return !(this->operator==(jsonBool));
  }

  Bool* Bool::clone() const {
    return new Bool(*this);
  }

  std::string Bool::toString() const {
    std::ostringstream oss;
    if(!this->isValid()) {
      oss << "JSON BOOL INVALID";
    } else if(this->boolValue==true) {
      oss << "true";
    } else if(this->boolValue==false) {
      oss << "false";
    } else {
      oss << "JSON BOOL INVALID";
    }
    return oss.str();
  }

  Type Bool::getType() const {
    return Type::BOOL;
  }

  bool Bool::isValid() const {
    return this->Value::isValid();
  }

  void Bool::setInvalid() {
    this->Value::setInvalid();
  }

  bool Bool::getValue() const {
    return this->boolValue;
  }

  std::ostream& operator<<(std::ostream& output, const Bool& jsonBool) {
    output << jsonBool.toString();
    return output;
  }
}
