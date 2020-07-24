// Receiver.cpp
// Receiver class implementation file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <algorithm>     // max
#include <array>         // array
#include <cstddef>       // NULL
#include <utility>       // move

// cote
#include <DateTime.hpp>  // DateTime
#include <Log.hpp>       // Log
#include <Receiver.hpp>   // Receiver
#include <utilities.hpp> // dB2Dec()

namespace cote {
  Receiver::Receiver(
   const std::array<double,3>& posn, const double& maxGainDB,
   const DateTime* const globalTime, const uint32_t& id, Log* const log
  ) : posn(posn), globalTime(globalTime), id(id), log(log) {
    this->setMaxGain(maxGainDB); // use setter b/c it contains logic
  }

  Receiver::Receiver(const Receiver& receiver) : posn(receiver.getPosn()),
   maxGainFactor(receiver.getMaxGain()), globalTime(receiver.getGlobalTime()),
   id(receiver.getID()), log(receiver.getLog()) {}

  Receiver::Receiver(Receiver&& receiver) : posn(receiver.posn),
   maxGainFactor(receiver.maxGainFactor), globalTime(receiver.globalTime),
   id(receiver.id), log(receiver.log) {
    receiver.globalTime = NULL;
    receiver.log = NULL;
  }

  Receiver::~Receiver() {
    this->globalTime = NULL;
    this->log = NULL;
  }

  Receiver& Receiver::operator=(const Receiver& receiver) {
    Receiver temp(receiver);
    *this = std::move(temp);
    return *this;
  }

  Receiver& Receiver::operator=(Receiver&& receiver) {
    this->posn = receiver.posn;
    this->maxGainFactor = receiver.maxGainFactor;
    this->globalTime = receiver.globalTime;
    this->id = receiver.id;
    this->log = receiver.log;
    receiver.globalTime = NULL;
    receiver.log = NULL;
    return *this;
  }

  Receiver* Receiver::clone() const {
    return new Receiver(*this);
  }

  std::array<double,3> Receiver::getPosn() const {
    return this->posn;
  }

  double Receiver::getMaxGain() const {
    return this->maxGainFactor;
  }

  const DateTime* Receiver::getGlobalTime() const {
    return this->globalTime;
  }

  uint32_t Receiver::getID() const {
    return this->id;
  }

  Log* Receiver::getLog() const {
    return this->log;
  }

  void Receiver::setPosn(const std::array<double,3>& posn) {
    this->posn = posn;
  }

  void Receiver::setMaxGain(const double& maxGainDB) {
    this->maxGainFactor = std::max(1.0,util::dB2Dec(maxGainDB));
  }

  void Receiver::setID(const uint32_t& id) {
    this->id = id;
  }

  void Receiver::update(const uint32_t& nanosecond) {
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    //this->localTime.update(nanosecond);
  }

  void Receiver::update(const uint8_t& second, const uint32_t& nanosecond) {
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    //this->localTime.update(second,nanosecond);
  }

  void Receiver::update(
   const uint8_t& minute, const uint8_t& second, const uint32_t& nanosecond
  ) {
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    //this->localTime.update(minute,second,nanosecond);
  }

  void Receiver::update(
   const uint8_t& hour, const uint8_t& minute, const uint8_t& second,
   const uint32_t& nanosecond
  ) {
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    //this->localTime.update(hour,minute,second,nanosecond);
  }
}
