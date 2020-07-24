// DateTime.cpp
// DateTime class implementation file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <algorithm>     // min, max
#include <iomanip>       // setfill, setw
#include <sstream>       // ostringstream
#include <utility>       // move

// cote library
#include <constants.hpp> // unit conversion constants
#include <DateTime.hpp>  // DateTime
#include <Log.hpp>       // Log
#include <LogLevel.hpp>  // LogLevel
#include <utilities.hpp> // isLeapYear

namespace cote {
  const uint8_t  DateTime::MONTH_MIN      =  1;
  const uint8_t  DateTime::MONTH_MAX      = 12;
  const uint8_t  DateTime::DAY_MIN        =  1;
  const uint8_t  DateTime::dayMax(const int16_t& year, const uint8_t& month) {
    uint8_t dayMaxes[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    uint8_t dayMax = dayMaxes[month-1];
    if((month==2) && util::isLeapYear(year)) {
      dayMax+=1;
    }
    return dayMax;
  }
  const uint8_t  DateTime::HOUR_MIN       =  0;
  const uint8_t  DateTime::HOUR_MAX       = 23;
  const uint8_t  DateTime::MINUTE_MIN     =  0;
  const uint8_t  DateTime::MINUTE_MAX     = 59;
  const uint8_t  DateTime::SECOND_MIN     =  0;
  const uint8_t  DateTime::SECOND_MAX     = 59;
  const uint32_t DateTime::NANOSECOND_MIN =         0;
  const uint32_t DateTime::NANOSECOND_MAX = 999999999;

  DateTime::DateTime(
   const int16_t& year, const uint8_t& month, const uint8_t& day,
   const uint8_t& hour, const uint8_t& minute, const uint8_t& second,
   const uint32_t& nanosecond, Log* const log
  ) : log(log) {
    this->year = year;
    this->month = std::max(MONTH_MIN,std::min(month,MONTH_MAX));
    this->day = std::max(DAY_MIN,std::min(day,dayMax(this->year,this->month)));
    this->hour = std::max(HOUR_MIN,std::min(hour,HOUR_MAX));
    this->minute = std::max(MINUTE_MIN,std::min(minute,MINUTE_MAX));
    this->second = std::max(SECOND_MIN,std::min(second,SECOND_MAX));
    this->nanosecond =
     std::max(NANOSECOND_MIN,std::min(nanosecond,NANOSECOND_MAX));
  }

  DateTime::DateTime(const DateTime& dateTime) : year(dateTime.getYear()),
   month(dateTime.getMonth()), day(dateTime.getDay()), hour(dateTime.getHour()),
   minute(dateTime.getMinute()), second(dateTime.getSecond()),
   nanosecond(dateTime.getNanosecond()), log(dateTime.getLog()) {}

  DateTime::DateTime(DateTime&& dateTime) : year(dateTime.year),
   month(dateTime.month), day(dateTime.day), hour(dateTime.hour),
   minute(dateTime.minute), second(dateTime.second),
   nanosecond(dateTime.nanosecond), log(dateTime.log) {
    dateTime.log = NULL;
  }

  DateTime::~DateTime() {
    this->log = NULL;
  }

  DateTime& DateTime::operator=(const DateTime& dateTime) {
    DateTime temp(dateTime);
    *this = std::move(temp);
    return *this;
  }

  DateTime& DateTime::operator=(DateTime&& dateTime) {
    this->year = dateTime.year;
    this->month = dateTime.month;
    this->day = dateTime.day;
    this->hour = dateTime.hour;
    this->minute = dateTime.minute;
    this->second = dateTime.second;
    this->nanosecond = dateTime.nanosecond;
    this->log = dateTime.log;
    dateTime.log = NULL;
    return *this;
  }

  DateTime* DateTime::clone() const {
    return new DateTime(*this);
  }

  int16_t DateTime::getYear() const {
    return this->year;
  }

  uint8_t DateTime::getMonth() const {
    return this->month;
  }

  uint8_t DateTime::getDay() const {
    return this->day;
  }

  uint8_t DateTime::getHour() const {
    return this->hour;
  }

  uint8_t DateTime::getMinute() const {
    return this->minute;
  }

  uint8_t DateTime::getSecond() const {
    return this->second;
  }

  uint32_t DateTime::getNanosecond() const {
    return this->nanosecond;
  }

  Log* DateTime::getLog() const {
    return this->log;
  }

  std::string DateTime::toString() const {
    std::ostringstream oss;
    oss << this->year << "-"
        << std::setfill('0') << std::setw(2)
        << static_cast<uint16_t>(this->month) << "-"
        << std::setfill('0') << std::setw(2)
        << static_cast<uint16_t>(this->day) << "T"
        << std::setfill('0') << std::setw(2)
        << static_cast<uint16_t>(this->hour) << ":"
        << std::setfill('0') << std::setw(2)
        << static_cast<uint16_t>(this->minute) << ":"
        << std::setfill('0') << std::setw(2)
        << static_cast<uint16_t>(this->second) << "."
        << std::setfill('0') << std::setw(9)
        << this->nanosecond;
    return oss.str();
  }

  void DateTime::update(const uint32_t& nanosecond) {
    uint32_t sanitized_ns =
     std::max(NANOSECOND_MIN,std::min(nanosecond,NANOSECOND_MAX));
    this->nanosecond += sanitized_ns;
    if(this->nanosecond > NANOSECOND_MAX) {
      this->nanosecond = this->nanosecond%cnst::NS_PER_SEC;
      this->second += 1;
      if(this->second > SECOND_MAX) {
        this->second = this->second%cnst::SEC_PER_MIN;
        this->minute += 1;
        if(this->minute > MINUTE_MAX) {
          this->minute = this->minute%cnst::MIN_PER_HOUR;
          this->hour += 1;
          if(this->hour > HOUR_MAX) {
            this->hour = this->hour%cnst::HOUR_PER_DAY;
            this->day += 1;
            if(this->day > dayMax(this->year,this->month)) {
              this->day = this->day%dayMax(this->year,this->month);
              this->month += 1;
              if(this->month > MONTH_MAX) {
                this->month = this->month%cnst::MONTH_PER_YEAR;
                this->year += 1;
              }
            }
          }
        }
      }
    }
  }

  void DateTime::update(const uint8_t& second, const uint32_t& nanosecond) {
    uint32_t sanitized_ns =
     std::max(NANOSECOND_MIN,std::min(nanosecond,NANOSECOND_MAX));
    uint8_t sanitized_sec = std::max(SECOND_MIN,std::min(second,SECOND_MAX));
    this->nanosecond += sanitized_ns;
    if(this->nanosecond > NANOSECOND_MAX) {
      this->nanosecond = this->nanosecond%cnst::NS_PER_SEC;
      this->second += 1;
    }
    this->second += sanitized_sec;
    if(this->second > SECOND_MAX) {
      this->second = this->second%cnst::SEC_PER_MIN;
      this->minute += 1;
      if(this->minute > MINUTE_MAX) {
        this->minute = this->minute%cnst::MIN_PER_HOUR;
        this->hour += 1;
        if(this->hour > HOUR_MAX) {
          this->hour = this->hour%cnst::HOUR_PER_DAY;
          this->day += 1;
          if(this->day > dayMax(this->year,this->month)) {
            this->day = this->day%dayMax(this->year,this->month);
            this->month += 1;
            if(this->month > MONTH_MAX) {
              this->month = this->month%cnst::MONTH_PER_YEAR;
              this->year += 1;
            }
          }
        }
      }
    }
  }

  void DateTime::update(
   const uint8_t& minute, const uint8_t& second, const uint32_t& nanosecond
  ) {
    uint32_t sanitized_ns =
     std::max(NANOSECOND_MIN,std::min(nanosecond,NANOSECOND_MAX));
    uint8_t sanitized_sec = std::max(SECOND_MIN,std::min(second,SECOND_MAX));
    uint8_t sanitized_min = std::max(MINUTE_MIN,std::min(minute,MINUTE_MAX));
    this->nanosecond += sanitized_ns;
    if(this->nanosecond > NANOSECOND_MAX) {
      this->nanosecond = this->nanosecond%cnst::NS_PER_SEC;
      this->second += 1;
    }
    this->second += sanitized_sec;
    if(this->second > SECOND_MAX) {
      this->second = this->second%cnst::SEC_PER_MIN;
      this->minute += 1;
    }
    this->minute += sanitized_min;
    if(this->minute > MINUTE_MAX) {
      this->minute = this->minute%cnst::MIN_PER_HOUR;
      this->hour += 1;
      if(this->hour > HOUR_MAX) {
        this->hour = this->hour%cnst::HOUR_PER_DAY;
        this->day += 1;
        if(this->day > dayMax(this->year,this->month)) {
          this->day = this->day%dayMax(this->year,this->month);
          this->month += 1;
          if(this->month > MONTH_MAX) {
            this->month = this->month%cnst::MONTH_PER_YEAR;
            this->year += 1;
          }
        }
      }
    }
  }

  void DateTime::update(
   const uint8_t& hour, const uint8_t& minute, const uint8_t& second,
   const uint32_t& nanosecond
  ) {
    uint32_t sanitized_ns =
     std::max(NANOSECOND_MIN,std::min(nanosecond,NANOSECOND_MAX));
    uint8_t sanitized_sec = std::max(SECOND_MIN,std::min(second,SECOND_MAX));
    uint8_t sanitized_min = std::max(MINUTE_MIN,std::min(minute,MINUTE_MAX));
    uint8_t sanitized_hour = std::max(HOUR_MIN,std::min(hour,HOUR_MAX));
    this->nanosecond += sanitized_ns;
    if(this->nanosecond > NANOSECOND_MAX) {
      this->nanosecond = this->nanosecond%cnst::NS_PER_SEC;
      this->second += 1;
    }
    this->second += sanitized_sec;
    if(this->second > SECOND_MAX) {
      this->second = this->second%cnst::SEC_PER_MIN;
      this->minute += 1;
    }
    this->minute += sanitized_min;
    if(this->minute > MINUTE_MAX) {
      this->minute = this->minute%cnst::MIN_PER_HOUR;
      this->hour += 1;
    }
    this->hour += sanitized_hour;
    if(this->hour > HOUR_MAX) {
      this->hour = this->hour%cnst::HOUR_PER_DAY;
      this->day += 1;
      if(this->day > dayMax(this->year,this->month)) {
        this->day = this->day%dayMax(this->year,this->month);
        this->month += 1;
        if(this->month > MONTH_MAX) {
          this->month = this->month%cnst::MONTH_PER_YEAR;
          this->year += 1;
        }
      }
    }
  }
}
