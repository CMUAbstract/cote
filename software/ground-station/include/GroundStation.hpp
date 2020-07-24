// GroundStation.hpp
// GroundStation class header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef COTE_GROUND_STATION_HPP
#define COTE_GROUND_STATION_HPP

// Standard library
#include <array>         // array

// cote
#include <DateTime.hpp>  // DateTime
#include <ISim.hpp>      // Simulator interface
#include <Log.hpp>       // Log

namespace cote {
  class GroundStation: public ISim {
  public:
    // lat: latitude in degrees; ranges from -90.0 (south) to +90.0 (north)
    //      Some functions in the cote utilities may not function properly at
    //      +/-90.0 degrees due to singularities
    // lon: longitude in degrees; ranges from -180.0 (west) to +180.0 (east)
    // hae: height above the ellipsoid in km
    GroundStation(
     const double& lat, const double& lon, const double& hae,
     const DateTime* const dateTime, const uint32_t& id=0, Log* const log=NULL
    );
    GroundStation(const GroundStation& groundStation);
    GroundStation(GroundStation&& groundStation);
    virtual ~GroundStation();
    virtual GroundStation& operator=(const GroundStation& groundStation);
    virtual GroundStation& operator=(GroundStation&& groundStation);
    virtual GroundStation* clone() const;
    double getLatitude() const;  // returns latitude in radians
    double getLongitude() const; // returns longitude in radians
    double getHAE() const;       // returns height above ellipsoid in kilometers
    std::array<double,3> getECIPosn() const;
    DateTime getLocalTime() const;
    const DateTime* getGlobalTime() const;
    uint32_t getID() const;
    Log* getLog() const;
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
    double lat;                   // latitude in radians
    double lon;                   // longitude in radians
    double hae;                   // height above ellipsoid in kilometers
    std::array<double,3> eciPosn; // (x,y,z) position in ECI coordinate frame
    DateTime  localTime;          // Shadow date/time can be used for fudging
    const DateTime* globalTime;   // singleton, should not be deleted
    uint32_t id;                  // ground station identification number
    Log* log;                     // singleton, should not be deleted
  };
}

#endif
