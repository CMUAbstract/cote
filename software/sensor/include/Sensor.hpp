// Sensor.hpp
// Sensor class header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef COTE_SENSOR_HPP
#define COTE_SENSOR_HPP

// Standard library
#include <array>        // array
#include <cstdint>      // uint32_t, uint64_t

// satsim
#include <DateTime.hpp> // DateTime
#include <ISim.hpp>     // Simulator interface
#include <Log.hpp>      // Log

namespace cote {
  class Sensor: public ISim {
  public:
    Sensor(
     const std::array<double,3>& eciPosn, const DateTime* const globalTime,
     const uint32_t& id=0, Log* const log=NULL
    );
    Sensor(const Sensor& sensor);
    Sensor(Sensor&& sensor);
    virtual ~Sensor();
    virtual Sensor& operator=(const Sensor& sensor);
    virtual Sensor& operator=(Sensor&& sensor);
    virtual Sensor* clone() const;
    bool getSenseTrigger() const;
    uint64_t getBitsBuffered() const;
    uint64_t getBitsPerSense() const;
    std::array<double,3> getPrevSensePosn() const;
    DateTime getPrevSenseDateTime() const;
    std::array<double,3> getECIPosn() const;
    const DateTime* getGlobalTime() const;
    uint32_t getID() const;
    Log* getLog() const;
    void triggerSense(); // sets senseTrigger to true
    uint64_t drainBuffer(const uint64_t& bits); // returns value <= bits
    void setBitsPerSense(const uint64_t& bits);
    void setECIPosn(const std::array<double,3>& eciPosn);
    void setPrevSensePosnDateTime(
     const std::array<double,3>& eciPosn, const DateTime& dateTime
    ); // setting the previous sense position and date/time should be atomic
       // it may need to be set without triggering a data collect (thus public)
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
    bool senseTrigger;                  // If true, sense event at next update
    uint64_t bitsBuffered;              // bits of data buffered on device
    uint64_t bitsPerSense;              // bits per sense event
    std::array<double,3> prevSensePosn; // ECI position of most recent sense
    DateTime prevSenseDateTime;         // Date and time of most recent sense
    std::array<double,3> eciPosn;       // current ECI position
    const DateTime* globalTime;         // singleton, should not be deleted
    uint32_t id;                        // sensor identification number
    Log* log;                           // singleton, should not be deleted
  };
}

#endif
