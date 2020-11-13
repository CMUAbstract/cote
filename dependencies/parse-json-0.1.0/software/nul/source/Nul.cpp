// Nul.cpp
// Nul implementation file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstddef>   // nullptr_t
#include <ostream>   // ostream
#include <sstream>   // ostringstream
#include <string>    // string

// json library
#include <Nul.hpp>   // Nul
#include <Type.hpp>  // Type
#include <Value.hpp> // Value

namespace json {
  Nul::Nul() : Value() {
    this->setInvalid();
  }

  Nul::Nul(std::string& malleableString) : Value() {
    if(malleableString.substr(0,4)=="null") {
      malleableString.erase(0,4);
    } else {
      this->setInvalid();
    }
  }

  Nul::Nul(const Nul& jsonNul) : Value(jsonNul) {}

  Nul::Nul(Nul&& jsonNul) : Value(jsonNul) {}

  Nul::~Nul() {}

  Value& Nul::operator=(const Value& value) {
    if(this!=&value && this->getType()==value.getType()) {
      if(!value.isValid()) {
        this->setInvalid();
      }
    } else if(this->getType()!=value.getType()) {
      this->setInvalid();
    }
    return *this;
  }

  Nul& Nul::operator=(const Nul& jsonNul) {
    if(this!=&jsonNul) {
      if(!jsonNul.isValid()) {
        this->setInvalid();
      }
    }
    return *this;
  }

  bool Nul::operator==(const Value& value) const {
    return (this->isValid()==value.isValid()) &&
           (this->getType()==value.getType());
  }

  bool Nul::operator==(const Nul& jsonNul) const {
    return (this->isValid()==jsonNul.isValid());
  }

  bool Nul::operator!=(const Value& value) const {
    return !(this->operator==(value));
  }

  bool Nul::operator!=(const Nul& jsonNul) const {
    return !(this->operator==(jsonNul));
  }

  Nul* Nul::clone() const {
    return new Nul(*this);
  }

  std::string Nul::toString() const {
    std::ostringstream oss;
    if(!this->isValid()) {
      oss << "JSON NUL INVALID";
    } else {
      oss << "null";
    }
    return oss.str();
  }

  Type Nul::getType() const {
    return Type::NUL;
  }

  bool Nul::isValid() const {
    return this->Value::isValid();
  }

  void Nul::setInvalid() {
    this->Value::setInvalid();
  }

  std::nullptr_t Nul::getValue() const {
    return nullptr;
  }

  std::ostream& operator<<(std::ostream& output, const Nul& jsonNul) {
    output << jsonNul.toString();
    return output;
  }
}
