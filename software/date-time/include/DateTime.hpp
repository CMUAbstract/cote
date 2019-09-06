// DateTime.hpp
// DateTime class header file
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

#ifndef COTE_DATE_TIME_HPP
#define COTE_DATE_TIME_HPP

// Standard library
#include <cstdint>  // uint8_t, uint32_t, int16_t
#include <string>   // string

// cote library
#include <ISim.hpp> // ISim
#include <Log.hpp>  // Log

namespace cote {
  class DateTime : public ISim {
  public:
    DateTime(
     const int16_t& year, const uint8_t& month, const uint8_t& day,
     const uint8_t& hour, const uint8_t& minute, const uint8_t& second,
     const uint32_t& nanosecond, Log* const log=NULL
    );
    DateTime(const DateTime& dateTime);
    DateTime(DateTime&& dateTime);
    virtual ~DateTime();
    virtual DateTime& operator=(const DateTime& dateTime);
    virtual DateTime& operator=(DateTime&& dateTime);
    virtual DateTime* clone() const;
    int16_t getYear() const;
    uint8_t getMonth() const;
    uint8_t getDay() const;
    uint8_t getHour() const;
    uint8_t getMinute() const;
    uint8_t getSecond() const;
    uint32_t getNanosecond() const;
    Log* getLog() const;
    std::string toString() const;
    virtual void update(const uint32_t& nanosecond);
    virtual void update(const uint8_t& second, const uint32_t& nanosecond);
    virtual void update(
     const uint8_t& minute, const uint8_t& second, const uint32_t& nanosecond
    );
    virtual void update(
     const uint8_t& hour, const uint8_t& minute, const uint8_t& second,
     const uint32_t& nanosecond
    );
  private:
    static const uint8_t  MONTH_MIN;
    static const uint8_t  MONTH_MAX;
    static const uint8_t  DAY_MIN;
    static const uint8_t  dayMax(const int16_t& year, const uint8_t& month);
    static const uint8_t  HOUR_MIN;
    static const uint8_t  HOUR_MAX;
    static const uint8_t  MINUTE_MIN;
    static const uint8_t  MINUTE_MAX;
    static const uint8_t  SECOND_MIN;
    static const uint8_t  SECOND_MAX;
    static const uint32_t NANOSECOND_MIN;
    static const uint32_t NANOSECOND_MAX;
    int16_t  year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
    uint32_t nanosecond;
    Log* log; // singleton, should not be deleted
  };
}

#endif
