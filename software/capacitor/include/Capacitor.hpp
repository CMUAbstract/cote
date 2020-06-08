// Capacitor.hpp
// Capacitor class header file
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

#ifndef COTE_CAPACITOR_HPP
#define COTE_CAPACITOR_HPP

// Standard library
#include <cstdint> // uint32_t

// cote
#include <Log.hpp> // Log

namespace cote {
  class Capacitor {
  public:
    Capacitor(
     const double& capacitanceFarad, const double& esrOhm, const uint32_t& id=0,
     Log* const log=NULL
    );
    Capacitor(const Capacitor& capacitor);
    Capacitor(Capacitor&& capacitor);
    virtual ~Capacitor();
    virtual Capacitor& operator=(const Capacitor& capacitor);
    virtual Capacitor& operator=(Capacitor&& capacitor);
    virtual Capacitor* clone() const;
    double getCapacitanceFarad() const;
    double getEsrOhm() const;
    double getChargeCoulomb() const;
    double setChargeCoulomb(const double& chargeCoulomb);
    uint32_t getID() const;
    Log* getLog() const;
  private:
    double capacitanceFarad; // capacitance of capacitor in Farad
    double esrOhm;           // equivalent series resistance in Ohm
    double chargeCoulomb;    // instantaneous charge of capacitor in Coulomb
    uint32_t id;             // identification number
    Log* log;                // singleton, should not be deleted
  };
}

#endif
