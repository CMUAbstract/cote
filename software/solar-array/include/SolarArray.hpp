// SolarArray.hpp
// SolarArray class header file
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

#ifndef COTE_SOLAR_ARRAY_HPP
#define COTE_SOLAR_ARRAY_HPP

// Standard library
#include <cstdint> // uint32_t

// cote
#include <Log.hpp> // Log

namespace cote {
  class SolarArray {
  public:
    SolarArray(
     const double& openCircuitVoltage, const double& surfaceAreaM2,
     const double& efficiency, const uint32_t& id=0, Log* const log=NULL
    );
    SolarArray(const SolarArray& solarArray);
    SolarArray(SolarArray&& solarArray);
    virtual ~SolarArray();
    virtual SolarArray& operator=(const SolarArray& solarArray);
    virtual SolarArray& operator=(SolarArray&& solarArray);
    virtual SolarArray* clone() const;
    double getOpenCircuitVoltage() const;
    double getSurfaceAreaM2() const;
    double getEfficiency() const;
    double getCurrentAmpere() const;
    void setCurrentAmpere(const double& currentAmpere);
    void setIrradianceWpM2(const double& irradianceWpM2);
    uint32_t getID() const;
    Log* getLog() const;
  private:
    double openCircuitVoltage; // solar array open circuit voltage
    double surfaceAreaM2;      // solar array surface area
    double efficiency;         // solar array efficiency
    double currentAmpere;      // solar array current in ampere
    uint32_t id;               // identification number
    Log* log;                  // singleton, should not be deleted
  };
}

#endif
