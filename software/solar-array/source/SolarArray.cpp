// SolarArray.cpp
// SolarArray class implementation file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstdint>        // uint32_t
#include <utility>        // move

// comsim
#include <SolarArray.hpp> // SolarArray
#include <Log.hpp>        // Log

namespace cote {
  SolarArray::SolarArray(
   const double& openCircuitVoltage, const double& surfaceAreaM2,
   const double& efficiency, const uint32_t& id, Log* const log
  ) : openCircuitVoltage(openCircuitVoltage), surfaceAreaM2(surfaceAreaM2),
      efficiency(efficiency), currentAmpere(0.0), id(id), log(log) {}

  SolarArray::SolarArray(const SolarArray& solarArray) :
   openCircuitVoltage(solarArray.getOpenCircuitVoltage()),
   surfaceAreaM2(solarArray.getSurfaceAreaM2()),
   efficiency(solarArray.getEfficiency()),
   currentAmpere(solarArray.getCurrentAmpere()), id(solarArray.getID()),
   log(solarArray.getLog()) {}

  SolarArray::SolarArray(SolarArray&& solarArray) :
   openCircuitVoltage(solarArray.openCircuitVoltage),
   surfaceAreaM2(solarArray.surfaceAreaM2), efficiency(solarArray.efficiency),
   currentAmpere(solarArray.currentAmpere), id(solarArray.id),
   log(solarArray.log) {
    solarArray.log = NULL;
  }

  SolarArray::~SolarArray() {
    this->log = NULL;
  }

  SolarArray& SolarArray::operator=(
   const SolarArray& solarArray
  ) {
    SolarArray temp(solarArray);
    *this = std::move(temp);
    return *this;
  }

  SolarArray& SolarArray::operator=(
   SolarArray&& solarArray
  ) {
    this->openCircuitVoltage = solarArray.openCircuitVoltage;
    this->surfaceAreaM2      = solarArray.surfaceAreaM2;
    this->efficiency         = solarArray.efficiency;
    this->currentAmpere      = solarArray.currentAmpere;
    this->id                 = solarArray.id;
    this->log                = solarArray.log;
    solarArray.log           = NULL;
    return *this;
  }

  SolarArray* SolarArray::clone() const {
    return new SolarArray(*this);
  }

  double SolarArray::getOpenCircuitVoltage() const {
    return this->openCircuitVoltage;
  }

  double SolarArray::getSurfaceAreaM2() const {
    return this->surfaceAreaM2;
  }

  double SolarArray::getEfficiency() const {
    return this->efficiency;
  }

  double SolarArray::getCurrentAmpere() const {
    return this->currentAmpere;
  }

  void SolarArray::setCurrentAmpere(const double& currentAmpere) {
    this->currentAmpere = currentAmpere;
  }

  void SolarArray::setIrradianceWpM2(const double& irradianceWpM2) {
    this->currentAmpere =
     (irradianceWpM2*this->surfaceAreaM2*this->efficiency)/
     this->openCircuitVoltage;
  }

  uint32_t SolarArray::getID() const {
    return this->id;
  }

  Log* SolarArray::getLog() const {
    return this->log;
  }
}
