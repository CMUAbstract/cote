// Receiver.hpp
// Receiver class header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef COTE_RECEIVER_HPP
#define COTE_RECEIVER_HPP

// Standard library
#include <array>        // array

// cote
#include <DateTime.hpp> // DateTime
#include <ISim.hpp>     // Simulation interface
#include <Log.hpp>      // Log

namespace cote {
  class Receiver: public ISim {
  public:
    Receiver(
     const std::array<double,3>& posn, const double& maxGainDB,
     const DateTime* const globalTime, const uint32_t& id=0, Log* const log=NULL
    );
    Receiver(const Receiver& receiver);
    Receiver(Receiver&& receiver);
    virtual ~Receiver();
    virtual Receiver& operator=(const Receiver& receiver);
    virtual Receiver& operator=(Receiver&& receiver);
    virtual Receiver* clone() const;
    std::array<double,3> getPosn() const;
    double getMaxGain() const; // Returns maximum value of gain factor (not dB)
    const DateTime* getGlobalTime() const;
    uint32_t getID() const;
    Log* getLog() const;
    void setPosn(const std::array<double,3>& posn);
    void setMaxGain(const double& maxGainDB);
    void setID(const uint32_t& id);
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
    std::array<double,3> posn;  // Three-dimensional position of RX antenna
    double maxGainFactor;       // maximum TX antenna gain (multiplicative)
    const DateTime* globalTime; // singleton, should not be deleted
    uint32_t id;                // identification number
    Log* log;                   // singleton, should not be deleted
  };
}

#endif
