// Transmitter.cpp
// Transmitter class implementation file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <algorithm>       // max, min
#include <array>           // array
#include <cstddef>         // NULL
#include <utility>         // move

// cote
#include <DateTime.hpp>    // DateTime
#include <Log.hpp>         // Log
#include <Transmitter.hpp> // Transmitter
#include <utilities.hpp>   // dB2Dec()

namespace cote {
  Transmitter::Transmitter(
   const std::array<double,3>& posn, const double& powerW,
   const double& lineLossDB, const double& maxGainDB,
   const DateTime* const globalTime, const uint32_t& id, Log* const log
  ) : posn(posn), globalTime(globalTime), id(id), log(log) {
    this->setPower(powerW);        // use setters
    this->setLineLoss(lineLossDB); // because
    this->setMaxGain(maxGainDB);   // they contain logic
  }

  Transmitter::Transmitter(const Transmitter& transmitter) :
   posn(transmitter.getPosn()), powerW(transmitter.getPower()),
   lineLossFactor(transmitter.getLineLoss()),
   maxGainFactor(transmitter.getMaxGain()),
   globalTime(transmitter.getGlobalTime()), id(transmitter.getID()),
   log(transmitter.getLog()) {}

  Transmitter::Transmitter(Transmitter&& transmitter) : posn(transmitter.posn),
   powerW(transmitter.powerW), lineLossFactor(transmitter.lineLossFactor),
   maxGainFactor(transmitter.maxGainFactor), globalTime(transmitter.globalTime),
   id(transmitter.id), log(transmitter.log) {
    transmitter.globalTime = NULL;
    transmitter.log = NULL;
  }

  Transmitter::~Transmitter() {
    this->globalTime = NULL;
    this->log = NULL;
  }

  Transmitter& Transmitter::operator=(const Transmitter& transmitter) {
    Transmitter temp(transmitter);
    *this = std::move(temp);
    return *this;
  }

  Transmitter& Transmitter::operator=(Transmitter&& transmitter) {
    this->posn = transmitter.posn;
    this->powerW = transmitter.powerW;
    this->lineLossFactor = transmitter.lineLossFactor;
    this->maxGainFactor = transmitter.maxGainFactor;
    this->globalTime = transmitter.globalTime;
    this->id = transmitter.id;
    this->log = transmitter.log;
    transmitter.globalTime = NULL;
    transmitter.log = NULL;
    return *this;
  }

  Transmitter* Transmitter::clone() const {
    return new Transmitter(*this);
  }

  std::array<double,3> Transmitter::getPosn() const {
    return this->posn;
  }

  double Transmitter::getPower() const {
    return this->powerW;
  }

  double Transmitter::getLineLoss() const {
    return this->lineLossFactor;
  }

  double Transmitter::getMaxGain() const {
    return this->maxGainFactor;
  }

  const DateTime* Transmitter::getGlobalTime() const {
    return this->globalTime;
  }

  uint32_t Transmitter::getID() const {
    return this->id;
  }

  Log* Transmitter::getLog() const {
    return this->log;
  }

  void Transmitter::setPosn(const std::array<double,3>& posn) {
    this->posn = posn;
  }

  void Transmitter::setPower(const double& powerW) {
    this->powerW = std::max(0.0, powerW);
  }

  void Transmitter::setLineLoss(const double& lineLossDB) {
    this->lineLossFactor = std::max(0.0,std::min(util::dB2Dec(lineLossDB),1.0));
  }

  void Transmitter::setMaxGain(const double& maxGainDB) {
    this->maxGainFactor = std::max(1.0,util::dB2Dec(maxGainDB));
  }

  void Transmitter::setID(const uint32_t& id) {
    this->id = id;
  }

  void Transmitter::update(const uint32_t& nanosecond) {
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    //this->localTime.update(nanosecond);
  }

  void Transmitter::update(const uint8_t& second, const uint32_t& nanosecond) {
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    //this->localTime.update(second,nanosecond);
  }

  void Transmitter::update(
   const uint8_t& minute, const uint8_t& second, const uint32_t& nanosecond
  ) {
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    //this->localTime.update(minute,second,nanosecond);
  }

  void Transmitter::update(
   const uint8_t& hour, const uint8_t& minute, const uint8_t& second,
   const uint32_t& nanosecond
  ) {
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    //this->localTime.update(minute,second,nanosecond);
  }
}
