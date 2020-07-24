// Channel.cpp
// Channel class implementation file
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
#include <Channel.hpp>     // Channel
#include <constants.hpp>   // constants
#include <DateTime.hpp>    // DateTime
#include <Log.hpp>         // Log
#include <Receiver.hpp>    // Receiver
#include <Transmitter.hpp> // Transmitter
#include <utilities.hpp>   // util::calc...()

namespace cote {
  Channel::Channel(
   const Transmitter* const transmitter, const Receiver* const receiver,
   const double& centerFrequencyHz, const double& bandwidthHz,
   const DateTime* const globalTime, Log* const log
  ) : transmitter(transmitter), receiver(receiver), globalTime(globalTime),
      log(log) {
    this->setCenterFrequency(centerFrequencyHz);
    this->setBandwidth(bandwidthHz);
    this->setChannelType();
    this->rangeKm = util::magnitude(util::calcSeparationVector(
     this->receiver->getPosn(), this->transmitter->getPosn()
    ));
    this->atmosphericLossFactor = util::calcAtmosphericLoss();
    this->systemNoiseTemperatureK = util::calcSystemNoiseTemp(
     this->getChannelType(), this->centerFrequencyHz
    );
    this->maxBitsPerSec = util::calcMaxBitsPerSec(
     this->transmitter->getPower(), this->transmitter->getLineLoss(),
     this->transmitter->getMaxGain(), this->atmosphericLossFactor,
     this->receiver->getMaxGain(), this->centerFrequencyHz, this->rangeKm,
     this->systemNoiseTemperatureK, this->bandwidthHz
    );
    this->bitsPerSec = this->maxBitsPerSec;
  }

  Channel::Channel(const Channel& channel) :
   transmitter(channel.getTransmitter()), receiver(channel.getReceiver()),
   centerFrequencyHz(channel.getCenterFrequency()),
   bandwidthHz(channel.getBandwidth()), globalTime(channel.getGlobalTime()),
   log(channel.getLog()), rangeKm(channel.getRange()),
   atmosphericLossFactor(channel.getAtmosphericLoss()),
   systemNoiseTemperatureK(channel.getSystemNoiseTemperature()),
   maxBitsPerSec(channel.getMaxBitsPerSec()),
   bitsPerSec(channel.getBitsPerSec()) {
    this->setChannelType();
  }

  Channel::Channel(Channel&& channel) : transmitter(channel.transmitter),
   receiver(channel.receiver), centerFrequencyHz(channel.centerFrequencyHz),
   bandwidthHz(channel.bandwidthHz), globalTime(channel.globalTime),
   log(channel.log), channelType(channel.channelType), rangeKm(channel.rangeKm),
   atmosphericLossFactor(channel.atmosphericLossFactor),
   systemNoiseTemperatureK(channel.systemNoiseTemperatureK),
   maxBitsPerSec(channel.maxBitsPerSec), bitsPerSec(channel.bitsPerSec) {
    channel.transmitter = NULL;
    channel.receiver = NULL;
    channel.globalTime = NULL;
    channel.log = NULL;
  }

  Channel::~Channel() {
    this->transmitter = NULL;
    this->receiver = NULL;
    this->globalTime = NULL;
    this->log = NULL;
  }

  Channel& Channel::operator=(const Channel& channel) {
    Channel temp(channel);
    *this = std::move(temp);
    return *this;
  }

  Channel& Channel::operator=(Channel&& channel) {
    this->transmitter = channel.transmitter;
    this->receiver = channel.receiver;
    this->centerFrequencyHz = channel.centerFrequencyHz;
    this->bandwidthHz = channel.bandwidthHz;
    this->globalTime = channel.globalTime;
    this->log = channel.log;
    this->channelType = channel.channelType;
    this->rangeKm = channel.rangeKm;
    this->atmosphericLossFactor = channel.atmosphericLossFactor;
    this->systemNoiseTemperatureK = channel.systemNoiseTemperatureK;
    this->maxBitsPerSec = channel.maxBitsPerSec;
    this->bitsPerSec = channel.bitsPerSec;
    channel.transmitter = NULL;
    channel.receiver = NULL;
    channel.globalTime = NULL;
    channel.log = NULL;
    return *this;
  }

  Channel* Channel::clone() const {
    return new Channel(*this);
  }

  const Transmitter* Channel::getTransmitter() const {
    return this->transmitter;
  }

  const Receiver* Channel::getReceiver() const {
    return this->receiver;
  }

  double Channel::getCenterFrequency() const {
    return this->centerFrequencyHz;
  }

  double Channel::getBandwidth() const {
    return this->bandwidthHz;
  }

  const DateTime* Channel::getGlobalTime() const {
    return this->globalTime;
  }

  Log* Channel::getLog() const {
    return this->log;
  }

  uint8_t Channel::getChannelType() const {
    switch(this->channelType) {
      case ChannelType::DOWNLINK:
        return cnst::CHANNEL_TYPE_DOWNLINK;
        break;
      case ChannelType::CROSSLINK:
        return cnst::CHANNEL_TYPE_CROSSLINK;
        break;
      case ChannelType::UPLINK:
        return cnst::CHANNEL_TYPE_UPLINK;
        break;
      case ChannelType::UNSPECIFIED:
        return static_cast<uint8_t>(ChannelType::UNSPECIFIED);
        break;
      default:
        return static_cast<uint8_t>(ChannelType::UNSPECIFIED);
        break;
    }
  }

  double Channel::getRange() const {
    return this->rangeKm;
  }

  double Channel::getAtmosphericLoss() const {
    return this->atmosphericLossFactor;
  }

  double Channel::getSystemNoiseTemperature() const {
    return this->systemNoiseTemperatureK;
  }

  uint64_t Channel::getMaxBitsPerSec() const {
    return this->maxBitsPerSec;
  }

  uint64_t Channel::getBitsPerSec() const {
    return this->bitsPerSec;
  }

  void Channel::setBitsPerSec(const uint64_t& bitsPerSec) {
    this->bitsPerSec = std::min(bitsPerSec, this->maxBitsPerSec);
  }

  void Channel::update(const uint32_t& nanosecond) {
    // stuff...
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    //this->localTime.update(nanosecond);
  }

  void Channel::update(const uint8_t& second, const uint32_t& nanosecond) {
    // stuff...
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    //this->localTime.update(second,nanosecond);
  }

  void Channel::update(
   const uint8_t& minute, const uint8_t& second, const uint32_t& nanosecond
  ) {
    // stuff...
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    //this->localTime.update(minute,second,nanosecond);
  }

  void Channel::update(
   const uint8_t& hour, const uint8_t& minute, const uint8_t& second,
   const uint32_t& nanosecond
  ) {
    // stuff...
    // **It is expected that this->globalTime has already been updated**
    // Perform (possibly custom) update for localTime
    //this->localTime.update(minute,second,nanosecond);
  }

  void Channel::setCenterFrequency(const double& centerFrequencyHz) {
    this->centerFrequencyHz = std::max(0.0, centerFrequencyHz);
  }

  void Channel::setBandwidth(const double& bandwidthHz) {
    this->bandwidthHz =
     std::max(0.0, std::min(bandwidthHz, this->centerFrequencyHz*2.0));
  }

  void Channel::setChannelType() {
    if(
     util::magnitude(this->transmitter->getPosn()) > cnst::WGS_84_A+98.0 &&
     util::magnitude(this->receiver->getPosn()) <= cnst::WGS_84_A+98.0
    ) {
      this->channelType = ChannelType::DOWNLINK;
    } else if(
     util::magnitude(this->transmitter->getPosn()) > cnst::WGS_84_A+98.0 &&
     util::magnitude(this->receiver->getPosn())  > cnst::WGS_84_A+98.0
    ) {
      this->channelType = ChannelType::CROSSLINK;
    } else if(
     util::magnitude(this->transmitter->getPosn())<= cnst::WGS_84_A+98.0 &&
     util::magnitude(this->receiver->getPosn())  > cnst::WGS_84_A+98.0
    ) {
      this->channelType = ChannelType::UPLINK;
    } else {
      this->channelType = ChannelType::UNSPECIFIED;
    }
  }
}
