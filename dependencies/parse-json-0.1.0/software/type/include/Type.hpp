// Type.hpp
// Type enum class header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef JSON_TYPE_HPP
#define JSON_TYPE_HPP

// Standard library
#include <cstdint> // uint8_t

// json library
// None

namespace json {
  enum class Type: uint8_t {
    ARRAY = 0,
    BOOL = 1,
    NUL = 2,
    NUMBER = 3,
    OBJECT = 4,
    STRING = 5
  };
}

#endif
