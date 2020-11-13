// Number.cpp
// Number implementation file
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
#include <Number.hpp>    // Number
#include <Type.hpp>      // Type
#include <json-util.hpp> // parseNumber
#include <Value.hpp>     // Value

namespace json {
  Number::Number() : Value() {
    this->setInvalid();
  }

  Number::Number(std::string& malleableString) : Value() {
    if(malleableString.size()>0) {
      char ch0 = malleableString.substr(0,1)[0];
      if(ch0=='0') {
        if(
         malleableString.size()>1 &&
         std::isdigit(malleableString.substr(1,1)[0])
        ) {
          this->setInvalid();
        } else {
          this->numberValue = util::parseNumber(malleableString);
        }
      } else if(ch0=='-' && malleableString.size()>1) {
        char ch1 = malleableString.substr(1,1)[0];
        if(ch1=='0') {
          if(
           malleableString.size()>2 &&
           std::isdigit(malleableString.substr(2,1)[0])
          ) {
            this->setInvalid();
          } else {
            this->numberValue = util::parseNumber(malleableString);
          }
        } else if(std::isdigit(ch1)) {
          this->numberValue = util::parseNumber(malleableString);
        } else {
          this->setInvalid();
        }
      } else if(std::isdigit(ch0)) {
        this->numberValue = util::parseNumber(malleableString);
      } else {
        this->setInvalid();
      }
    } else {
      this->setInvalid();
    }
  }

  Number::Number(const Number& jsonNumber) : Value(jsonNumber),
   numberValue(jsonNumber.getValue()) {}

  Number::Number(Number&& jsonNumber) : Value(jsonNumber),
   numberValue(jsonNumber.numberValue) {}

  Number::~Number() {}

  Value& Number::operator=(const Value& value) {
    if(this!=&value && this->getType()==value.getType()) {
      if(!value.isValid()) {
        this->setInvalid();
      }
      this->numberValue = dynamic_cast<const Number&>(value).getValue();
    } else if(this->getType()!=value.getType()) {
      this->setInvalid();
    }
    return *this;
  }

  Number& Number::operator=(const Number& jsonNumber) {
    if(this!=&jsonNumber) {
      if(!jsonNumber.isValid()) {
        this->setInvalid();
      }
      this->numberValue = jsonNumber.getValue();
    }
    return *this;
  }

  bool Number::operator==(const Value& value) const {
    return (this->isValid()==value.isValid()) &&
           (this->getType()==value.getType()) &&
           (this->numberValue==dynamic_cast<const Number&>(value).getValue());
  }

  bool Number::operator==(const Number& jsonNumber) const {
    return (this->isValid()==jsonNumber.isValid()) &&
           (this->numberValue==jsonNumber.getValue());
  }

  bool Number::operator!=(const Value& value) const {
    return !(this->operator==(value));
  }

  bool Number::operator!=(const Number& jsonNumber) const {
    return !(this->operator==(jsonNumber));
  }

  Number* Number::clone() const {
    return new Number(*this);
  }

  std::string Number::toString() const {
    std::ostringstream oss;
    if(!this->isValid()) {
      oss << "JSON NUMBER INVALID";
    } else {
      oss << this->numberValue;
    }
    return oss.str();
  }

  Type Number::getType() const {
    return Type::NUMBER;
  }

  bool Number::isValid() const {
    return this->Value::isValid();
  }

  void Number::setInvalid() {
    this->Value::setInvalid();
  }

  double Number::getValue() const {
    return this->numberValue;
  }

  std::ostream& operator<<(std::ostream& output, const Number& jsonNumber) {
    output << jsonNumber.toString();
    return output;
  }
}
