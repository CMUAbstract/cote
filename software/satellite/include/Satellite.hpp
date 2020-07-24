// Satellite.hpp
// Satellite class header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef COTE_SATELLITE_HPP
#define COTE_SATELLITE_HPP

// Standard library
#include <string>       // string

// cote library
#include <DateTime.hpp> // DateTime
#include <ISim.hpp>     // Simulator interface
#include <Log.hpp>      // Log

namespace cote {
  class Satellite: public ISim {
  public:
    Satellite(
     const std::string& tleFile, const DateTime* const globalTime,
     Log* const log=NULL
    );
    Satellite(const Satellite& satellite);
    Satellite(Satellite&& satellite);
    virtual ~Satellite();
    virtual Satellite& operator=(const Satellite& satellite);
    virtual Satellite& operator=(Satellite&& satellite);
    virtual Satellite* clone() const;
    uint32_t getID() const;
    DateTime getTLEEpoch() const;
    float getBstar() const;
    float getInclination() const;
    float getRAAN() const;
    float getEccentricity() const;
    float getArgOfPerigee() const;
    float getMeanAnomaly() const;
    float getMeanMotion() const;
    std::array<double,3> getECIPosn() const;
    DateTime getLocalTime() const;
    const DateTime* getGlobalTime() const;
    Log* getLog() const;
    void setID(const uint32_t& id);
    void setLocalTime(const DateTime& localTime);
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
    uint32_t id;                  // defaults to catalog number
    DateTime tleEpoch;            // validity of TLE is centered at this time
    float bstar;                  // inverse Earth radians
    float inclination;            // radians
    float raan;                   // right ascension of node in radians
    float eccentricity;           // unitless
    float argOfPerigee;           // radians
    float meanAnomaly;            // radians
    float meanMotion;             // radians per minute
    std::array<double,3> eciPosn; // ECI position at current local time
    DateTime localTime;           // To be used by subclasses for fudge tricks
    const DateTime* globalTime;   // singleton, should not be deleted
    Log* log;                     // singleton, should not be deleted
  };
}

#endif
