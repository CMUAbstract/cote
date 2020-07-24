// ISim.hpp
// ISim interace file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef COTE_ISIM_HPP
#define COTE_ISIM_HPP

// Standard library
#include <cstdint> // uint32_t, uint8_t

// cote library
// None

namespace cote {
  class ISim {
  public:
    virtual void update(const uint32_t& nanosecond) = 0;
    virtual void update(const uint8_t& second, const uint32_t& nanosecond) = 0;
    virtual void update(
     const uint8_t& minute, const uint8_t& second, const uint32_t& nanosecond
    ) = 0;
    virtual void update(
     const uint8_t& hour, const uint8_t& minute, const uint8_t& second,
     const uint32_t& nanosecond
    ) = 0;
  };
}

#endif
