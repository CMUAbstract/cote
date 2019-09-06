// ISim.hpp
// ISim interace file
//
// Written by Bradley Denby
// Other contributors: None
//
// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication with this
// software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

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
