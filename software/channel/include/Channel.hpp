// Channel.hpp
// Channel class header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef COTE_CHANNEL_HPP
#define COTE_CHANNEL_HPP

// Standard library
#include <array>           // array
#include <cstdint>         // uint64_t

// cote
#include <DateTime.hpp>    // DateTime
#include <ISim.hpp>        // Simulation interface
#include <Log.hpp>         // Log
#include <Receiver.hpp>    // Receiver
#include <Transmitter.hpp> // Transmitter

namespace cote {
  class Channel: public ISim {
  public:
    Channel(
     const Transmitter* const transmitter, const Receiver* const receiver,
     const double& centerFrequencyHz, const double& bandwidthHz,
     const DateTime* const globalTime, Log* const log=NULL
    );
    Channel(const Channel& channel);
    Channel(Channel&& channel);
    virtual ~Channel();
    virtual Channel& operator=(const Channel& channel);
    virtual Channel& operator=(Channel&& channel);
    virtual Channel* clone() const;
    const Transmitter* getTransmitter() const;
    const Receiver* getReceiver() const;
    double getCenterFrequency() const;        // Returns center frequency in Hz
    double getBandwidth() const;              // Returns bandwidth in Hz
    const DateTime* getGlobalTime() const;
    Log* getLog() const;
    uint8_t getChannelType() const;           // returns corresponding cnst
    double getRange() const;                  // Returns range in km
    double getAtmosphericLoss() const;        // Return air loss factor (not dB)
    double getSystemNoiseTemperature() const; // Returns system noise temp (K)
    uint64_t getMaxBitsPerSec() const;
    uint64_t getBitsPerSec() const;
    void setBitsPerSec(const uint64_t& bitsPerSec);
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
    enum class ChannelType : uint8_t {
      DOWNLINK    =   1,
      CROSSLINK   =   2,
      UPLINK      =   3,
      UNSPECIFIED = 255
    };
    void setCenterFrequency(const double& centerFrequencyHz);
    void setBandwidth(const double& bandwidthHz);
    void setChannelType();
    const Transmitter* transmitter; // TX
    const Receiver* receiver;       // RX
    double centerFrequencyHz;       // center frequency in Hz
    double bandwidthHz;             // bandwidth in Hz
    const DateTime* globalTime;     // singleton, should not be deleted
    Log* log;                       // singleton, should not be deleted
    ChannelType channelType;        // Downlink, crosslink, or uplink
    double rangeKm;                 // TX-RX separation vector magnitude in km
    double atmosphericLossFactor;   // signal power loss factor due to air
    double systemNoiseTemperatureK; // system noise temperature T_s in Kelvin
    uint64_t maxBitsPerSec;         // Shannon-Hartley theorem limit
    uint64_t bitsPerSec;            // actual data rate
  };
}

#endif
