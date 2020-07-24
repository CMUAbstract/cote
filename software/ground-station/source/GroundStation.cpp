// GroundStation.cpp
// GroundStation class implementation file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <algorithm>         // max, min
#include <array>             // array

// cote
#include <constants.hpp>     // constants
#include <DateTime.hpp>      // DateTime
#include <GroundStation.hpp> // GroundStation
#include <Log.hpp>           // Log
#include <utilities.hpp>     // utilities

namespace cote {
  GroundStation::GroundStation(
   const double& lat, const double& lon, const double& hae,
   const DateTime* const dateTime, const uint32_t& id, Log* const log
  ) : hae(hae), localTime(*dateTime), globalTime(dateTime), id(id), log(log) {
    this->lat =
     std::max(-1.0*cnst::HALF_PI,std::min(lat*cnst::RAD_PER_DEG,cnst::HALF_PI));
    this->lon =
     std::max(-1.0*cnst::PI,std::min(lon*cnst::RAD_PER_DEG,cnst::PI));
    const double JD = cote::util::calcJulianDayFromYMD(
     this->localTime.getYear(),this->localTime.getMonth(),
     this->localTime.getDay()
    );
    const uint32_t SEC = cote::util::calcSecSinceMidnight(
     this->localTime.getHour(),this->localTime.getMinute(),
     this->localTime.getSecond()
    );
    const uint32_t NS = this->localTime.getNanosecond();
    this->eciPosn = util::dtlla2eci(JD,SEC,NS,this->lat,this->lon,this->hae);
  }

  GroundStation::GroundStation(const GroundStation& groundStation) :
   lat(groundStation.getLatitude()), lon(groundStation.getLongitude()),
   hae(groundStation.getHAE()), eciPosn(groundStation.getECIPosn()),
   localTime(groundStation.getLocalTime()),
   globalTime(groundStation.getGlobalTime()), id(groundStation.getID()),
   log(groundStation.getLog()) {}

  GroundStation::GroundStation(GroundStation&& groundStation) :
   lat(groundStation.lat), lon(groundStation.lon), hae(groundStation.hae),
   eciPosn(groundStation.eciPosn), localTime(groundStation.localTime),
   globalTime(groundStation.globalTime), id(groundStation.id),
   log(groundStation.log) {
    groundStation.globalTime = NULL;
    groundStation.log = NULL;
  }

  GroundStation::~GroundStation() {
    this->globalTime = NULL;
    this->log = NULL;
  }

  GroundStation& GroundStation::operator=(const GroundStation& groundStation) {
    GroundStation temp(groundStation);
    *this = std::move(temp);
    return *this;
  }

  GroundStation& GroundStation::operator=(GroundStation&& groundStation) {
    this->lat = groundStation.lat;
    this->lon = groundStation.lon;
    this->hae = groundStation.hae;
    this->eciPosn = groundStation.eciPosn;
    this->localTime = groundStation.localTime;
    this->globalTime = groundStation.globalTime;
    this->id = groundStation.id;
    this->log = groundStation.log;
    groundStation.globalTime = NULL;
    groundStation.log = NULL;
    return *this;
  }

  GroundStation* GroundStation::clone() const {
    return new GroundStation(*this);
  }

  double GroundStation::getLatitude() const {
    return this->lat;
  }

  double GroundStation::getLongitude() const {
    return this->lon;
  }

  double GroundStation::getHAE() const {
    return this->hae;
  }

  std::array<double,3> GroundStation::getECIPosn() const {
    return this->eciPosn;
  }

  DateTime GroundStation::getLocalTime() const {
    return this->localTime;
  }

  const DateTime* GroundStation::getGlobalTime() const {
    return this->globalTime;
  }

  uint32_t GroundStation::getID() const {
    return this->id;
  }

  Log* GroundStation::getLog() const {
    return this->log;
  }

  void GroundStation::update(const uint32_t& nanosecond) {
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    this->localTime.update(nanosecond);
    // calculate eciPosn
    const double JD = cote::util::calcJulianDayFromYMD(
     this->localTime.getYear(),this->localTime.getMonth(),
     this->localTime.getDay()
    );
    const uint32_t SEC = cote::util::calcSecSinceMidnight(
     this->localTime.getHour(),this->localTime.getMinute(),
     this->localTime.getSecond()
    );
    const uint32_t NS = this->localTime.getNanosecond();
    this->eciPosn = util::dtlla2eci(JD,SEC,NS,this->lat,this->lon,this->hae);
  }

  void GroundStation::update(const uint8_t& second, const uint32_t& nanosecond){
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    this->localTime.update(second,nanosecond);
    // calculate eciPosn
    const double JD = cote::util::calcJulianDayFromYMD(
     this->localTime.getYear(),this->localTime.getMonth(),
     this->localTime.getDay()
    );
    const uint32_t SEC = cote::util::calcSecSinceMidnight(
     this->localTime.getHour(),this->localTime.getMinute(),
     this->localTime.getSecond()
    );
    const uint32_t NS = this->localTime.getNanosecond();
    this->eciPosn = util::dtlla2eci(JD,SEC,NS,this->lat,this->lon,this->hae);
  }

  void GroundStation::update(
   const uint8_t& minute, const uint8_t& second, const uint32_t& nanosecond
  ) {
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    this->localTime.update(minute,second,nanosecond);
    // calculate eciPosn
    const double JD = cote::util::calcJulianDayFromYMD(
     this->localTime.getYear(),this->localTime.getMonth(),
     this->localTime.getDay()
    );
    const uint32_t SEC = cote::util::calcSecSinceMidnight(
     this->localTime.getHour(),this->localTime.getMinute(),
     this->localTime.getSecond()
    );
    const uint32_t NS = this->localTime.getNanosecond();
    this->eciPosn = util::dtlla2eci(JD,SEC,NS,this->lat,this->lon,this->hae);
  }

  void GroundStation::update(
   const uint8_t& hour, const uint8_t& minute, const uint8_t& second,
   const uint32_t& nanosecond
  ) {
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    this->localTime.update(hour,minute,second,nanosecond);
    // calculate eciPosn
    const double JD = cote::util::calcJulianDayFromYMD(
     this->localTime.getYear(),this->localTime.getMonth(),
     this->localTime.getDay()
    );
    const uint32_t SEC = cote::util::calcSecSinceMidnight(
     this->localTime.getHour(),this->localTime.getMinute(),
     this->localTime.getSecond()
    );
    const uint32_t NS = this->localTime.getNanosecond();
    this->eciPosn = util::dtlla2eci(JD,SEC,NS,this->lat,this->lon,this->hae);
  }
}
