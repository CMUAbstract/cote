// Transmitter.hpp
// Transmitter class header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef COTE_TRANSMITTER_HPP
#define COTE_TRANSMITTER_HPP

// Standard library
#include <array>        // array

// cote
#include <DateTime.hpp> // DateTime
#include <ISim.hpp>     // Simulation interface
#include <Log.hpp>      // Log

namespace cote {
  class Transmitter: public ISim {
  public:
    Transmitter(
     const std::array<double,3>& posn, const double& powerW,
     const double& lineLossDB, const double& maxGainDB,
     const DateTime* const globalTime, const uint32_t& id=0, Log* const log=NULL
    );
    Transmitter(const Transmitter& transmitter);
    Transmitter(Transmitter&& transmitter);
    virtual ~Transmitter();
    virtual Transmitter& operator=(const Transmitter& transmitter);
    virtual Transmitter& operator=(Transmitter&& transmitter);
    virtual Transmitter* clone() const;
    std::array<double,3> getPosn() const;
    double getPower() const;    // Returns power in Watts
    double getLineLoss() const; // Returns line loss factor (not dB)
    double getMaxGain() const;  // Returns maximum value of gain factor (not dB)
    const DateTime* getGlobalTime() const;
    uint32_t getID() const;
    Log* getLog() const;
    void setPosn(const std::array<double,3>& posn);
    void setPower(const double& powerW);
    void setLineLoss(const double& lineLossDB);
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
    std::array<double,3> posn;  // Three-dimensional position of TX antenna
    double powerW;              // TX power in Watt
    double lineLossFactor;      // line loss between TX and ANT (multiplicative)
    double maxGainFactor;       // maximum TX antenna gain (multiplicative)
    const DateTime* globalTime; // singleton, should not be deleted
    uint32_t id;                // identification number
    Log* log;                   // singleton, should not be deleted
  };
}

#endif
