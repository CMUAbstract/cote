// Array.cpp
// Array implementation file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstddef>       // size_t
#include <ostream>       // ostream
#include <sstream>       // ostringstream
#include <string>        // string
#include <vector>        // vector

// json library
#include <Array.hpp>     // Array
#include <Type.hpp>      // Type
#include <json-util.hpp> // parseArray
#include <Value.hpp>     // Value

namespace json {
  Array::Array() : Value() {
    this->setInvalid();
  }

  Array::Array(std::string& malleableString) : Value() {
    if(malleableString.substr(0,1)=="[") {
      this->arrayValue = util::parseArray(malleableString);
    } else {
      this->setInvalid();
    }
  }

  Array::Array(const Array& jsonArray) : Value(jsonArray) {
    std::vector<Value*> jsonArrayValue = jsonArray.getValue();
    for(std::size_t i=0; i<jsonArrayValue.size(); i++) {
      this->arrayValue.push_back(jsonArrayValue.at(i)->clone());
    }
  }

  Array::Array(Array&& jsonArray) : Value(jsonArray) {
    for(std::size_t i=0; i<jsonArray.arrayValue.size(); i++) {
      this->arrayValue.push_back(jsonArray.arrayValue.at(i));
      jsonArray.arrayValue.at(i) = nullptr;
    }
  }

  Array::~Array() {
    for(std::size_t i=0; i<this->arrayValue.size(); i++) {
      delete this->arrayValue.at(i);
    }
  }

  Value& Array::operator=(const Value& value) {
    if(this!=&value && this->getType()==value.getType()) {
      if(!value.isValid()) {
        this->setInvalid();
      }
      // delete the current array
      for(std::size_t i=0; i<this->arrayValue.size(); i++) {
        delete this->arrayValue.at(i);
      }
      this->arrayValue.clear();
      // get the new array
      std::vector<Value*> otherArrayValue =
       dynamic_cast<const Array&>(value).getValue();
      // make a deep copy
      for(std::size_t i=0; i<otherArrayValue.size(); i++) {
        this->arrayValue.push_back(otherArrayValue.at(i)->clone());
      }
    } else if(this->getType()!=value.getType()) {
      this->setInvalid();
    }
    return *this;
  }

  Array& Array::operator=(const Array& jsonArray) {
    if(this!=&jsonArray) {
      if(!jsonArray.isValid()) {
        this->setInvalid();
      }
      // delete the current array
      for(std::size_t i=0; i<this->arrayValue.size(); i++) {
        delete this->arrayValue.at(i);
      }
      this->arrayValue.clear();
      // get the new array
      std::vector<Value*> otherArrayValue = jsonArray.getValue();
      // make a deep copy
      for(std::size_t i=0; i<otherArrayValue.size(); i++) {
        this->arrayValue.push_back(otherArrayValue.at(i)->clone());
      }
    }
    return *this;
  }

  bool Array::operator==(const Value& value) const {
    return (this->getType()==value.getType()) &&
           (this->arrayValue==dynamic_cast<const Array&>(value).getValue());
  }

  bool Array::operator==(const Array& jsonArray) const {
    return (this->arrayValue==jsonArray.getValue());
  }

  bool Array::operator!=(const Value& value) const {
    return !(this->operator==(value));
  }

  bool Array::operator!=(const Array& jsonArray) const {
    return !(this->operator==(jsonArray));
  }

  Array* Array::clone() const {
    return new Array(*this);
  }

  std::string Array::toString() const {
    std::ostringstream oss;
    if(!this->isValid()) {
      oss << "JSON ARRAY INVALID";
    } else {
      oss << "[";
      for(std::size_t i=0; i<this->arrayValue.size(); i++) {
        oss << std::endl
            << "  "
            << arrayValue.at(i)->toString();
        if(i+1<this->arrayValue.size()) {
          oss << ",";
        }
      }
      oss << std::endl
          << "]";
    }
    return oss.str();
  }

  Type Array::getType() const {
    return Type::ARRAY;
  }

  bool Array::isValid() const {
    return this->Value::isValid();
  }

  void Array::setInvalid() {
    this->Value::setInvalid();
  }

  std::vector<Value*> Array::getValue() const {
    return this->arrayValue;
  }

  std::ostream& operator<<(std::ostream& output, const Array& jsonArray) {
    output << jsonArray.toString();
    return output;
  }
}
