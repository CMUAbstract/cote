// Capacitor.cpp
// Capacitor class implementation file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstdint>       // uint32_t
#include <utility>       // move

// comsim
#include <Capacitor.hpp> // Capacitor
#include <Log.hpp>       // Log

namespace cote {
  Capacitor::Capacitor(
   const double& capacitanceFarad, const double& esrOhm, const uint32_t& id,
   Log* const log
  ) : capacitanceFarad(capacitanceFarad), esrOhm(esrOhm), chargeCoulomb(0.0),
      id(id), log(log) {}

  Capacitor::Capacitor(const Capacitor& capacitor) :
   capacitanceFarad(capacitor.getCapacitanceFarad()),
   esrOhm(capacitor.getEsrOhm()), chargeCoulomb(capacitor.getChargeCoulomb()),
   id(capacitor.getID()), log(capacitor.getLog()) {}

  Capacitor::Capacitor(Capacitor&& capacitor) :
   capacitanceFarad(capacitor.capacitanceFarad), esrOhm(capacitor.esrOhm),
   chargeCoulomb(capacitor.chargeCoulomb), id(capacitor.id),
   log(capacitor.log) {
    capacitor.log = NULL;
  }

  Capacitor::~Capacitor() {
    this->log = NULL;
  }

  Capacitor& Capacitor::operator=(const Capacitor& capacitor) {
    Capacitor temp(capacitor);
    *this = std::move(temp);
    return *this;
  }

  Capacitor& Capacitor::operator=(Capacitor&& capacitor) {
    this->capacitanceFarad = capacitor.capacitanceFarad;
    this->esrOhm           = capacitor.esrOhm;
    this->chargeCoulomb    = capacitor.chargeCoulomb;
    this->id               = capacitor.id;
    this->log              = capacitor.log;
    capacitor.log          = NULL;
    return *this;
  }

  Capacitor* Capacitor::clone() const {
    return new Capacitor(*this);
  }

  double Capacitor::getCapacitanceFarad() const {
    return this->capacitanceFarad;
  }

  double Capacitor::getEsrOhm() const {
    return this->esrOhm;
  }

  double Capacitor::getChargeCoulomb() const {
    return this->chargeCoulomb;
  }

  double Capacitor::setChargeCoulomb(const double& chargeCoulomb) {
    this->chargeCoulomb = chargeCoulomb;
    return this->chargeCoulomb;
  }

  uint32_t Capacitor::getID() const {
    return this->id;
  }

  Log* Capacitor::getLog() const {
    return this->log;
  }
}
