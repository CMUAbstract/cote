// Object.cpp
// Object implementation file
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

// json library
#include <Object.hpp>     // Object
#include <Type.hpp>      // Type
#include <json-util.hpp> // utilities
#include <Value.hpp>     // Value

namespace json {
  Object::Object() : Value() {
    this->setInvalid();
  }

  Object::Object(std::string& malleableString) : Value() {
    if(malleableString.substr(0,1)=="{") {
      this->objectValue = util::parseObject(malleableString);
    } else {
      this->setInvalid();
    }
  }

  Object::Object(const Object& jsonObject) : Value(jsonObject) {
    std::map<std::string,Value*> jsonObjectValue = jsonObject.getValue();
    for(
     std::map<std::string,Value*>::iterator it = jsonObjectValue.begin();
     it!=jsonObjectValue.end(); it++
    ) {
      this->objectValue.insert(std::make_pair(it->first,it->second));
    }
  }

  Object::Object(Object&& jsonObject) : Value(jsonObject) {
    for(
     std::map<std::string,Value*>::iterator it = jsonObject.objectValue.begin();
     it!=jsonObject.objectValue.end();
    ) {
      this->objectValue.insert(std::make_pair(it->first,it->second));
      it->second = nullptr;
      it = jsonObject.objectValue.erase(it);
    }
  }

  Object::~Object() {
    for(
     std::map<std::string,Value*>::iterator it = this->objectValue.begin();
     it!=this->objectValue.end(); it++
    ) {
      delete it->second;
    }
  }

  Value& Object::operator=(const Value& value) {
    if(this!=&value && this->getType()==value.getType()) {
      if(!value.isValid()) {
        this->setInvalid();
      }
      // delete the current object
      for(
       std::map<std::string,Value*>::iterator it = this->objectValue.begin();
       it!=this->objectValue.end(); it++
      ) {
        delete it->second;
      }
      this->objectValue.clear();
      // get the new object
      std::map<std::string,Value*> otherObjectValue =
       dynamic_cast<const Object&>(value).getValue();
      // make a deep copy
      for(
       std::map<std::string,Value*>::iterator it = otherObjectValue.begin();
       it!=otherObjectValue.end(); it++
      ) {
        this->objectValue.insert(std::make_pair(it->first,it->second));
      }
    } else if(this->getType()!=value.getType()) {
      this->setInvalid();
    }
    return *this;
  }

  Object& Object::operator=(const Object& jsonObject) {
    if(this!=&jsonObject) {
      if(!jsonObject.isValid()) {
        this->setInvalid();
      }
      // delete the current object
      for(
       std::map<std::string,Value*>::iterator it = this->objectValue.begin();
       it!=this->objectValue.end(); it++
      ) {
        delete it->second;
      }
      this->objectValue.clear();
      // get the new object
      std::map<std::string,Value*> otherObjectValue = jsonObject.getValue();
      // make a deep copy
      for(
       std::map<std::string,Value*>::iterator it = otherObjectValue.begin();
       it!=otherObjectValue.end(); it++
      ) {
        this->objectValue.insert(std::make_pair(it->first,it->second));
      }
    }
    return *this;
  }

  bool Object::operator==(const Value& value) const {
    return (this->getType()==value.getType()) &&
           (this->objectValue==dynamic_cast<const Object&>(value).getValue());
  }

  bool Object::operator==(const Object& jsonObject) const {
    return (this->objectValue==jsonObject.getValue());
  }

  bool Object::operator!=(const Value& value) const {
    return !(this->operator==(value));
  }

  bool Object::operator!=(const Object& jsonObject) const {
    return !(this->operator==(jsonObject));
  }

  Object* Object::clone() const {
    return new Object(*this);
  }

  std::string Object::toString() const {
    std::ostringstream oss;
    if(!this->isValid()) {
      oss << "JSON OBJECT INVALID";
    } else {
      oss << "{";
      std::size_t mapSize = this->objectValue.size();
      std::size_t itCount = 0;
      for(
       std::map<std::string,Value*>::const_iterator it =
        this->objectValue.begin(); it!=this->objectValue.end(); it++
      ) {
        oss << std::endl
            << "  "
            << "\"" << it->first << "\""
            << " : "
            << it->second->toString();
        if(itCount+1<mapSize) {
          oss << ",";
        }
        itCount++;
      }
      oss << std::endl
          << "}";
    }
    return oss.str();
  }

  Type Object::getType() const {
    return Type::ARRAY;
  }

  bool Object::isValid() const {
    return this->Value::isValid();
  }

  void Object::setInvalid() {
    this->Value::setInvalid();
  }

  std::map<std::string,Value*> Object::getValue() const {
    return this->objectValue;
  }

  std::ostream& operator<<(std::ostream& output, const Object& jsonObject) {
    output << jsonObject.toString();
    return output;
  }
}
