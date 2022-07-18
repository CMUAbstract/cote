// capture-and-downlink.cpp
//
// Simulates a bent-pipe satellite and logs data observed and downlinked
//
// Requires C++17 (e.g. GCC 8) for std::filesystem
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <algorithm>         // max
#include <cmath>             // round
#include <cstdlib>           // exit, EXIT_SUCCESS
#include <filesystem>        // path
#include <fstream>           // ifstream
#include <iostream>          // cout
#include <map>               // map
#include <ostream>           // endl
#include <string>            // string
#include <utility>           // make_pair

// cote
#include <Channel.hpp>       // Channel
#include <constants.hpp>     // WGS_84_A
#include <DateTime.hpp>      // DateTime
#include <GroundStation.hpp> // GroundStation
#include <Log.hpp>           // Log
#include <LogLevel.hpp>      // LogLevel
#include <Receiver.hpp>      // Receiver
#include <Satellite.hpp>     // Satellite
#include <Sensor.hpp>        // Sensor
#include <Transmitter.hpp>   // Transmitter
#include <utilities.hpp>     // calcJulianDayFromYMD, calcSecSinceMidnight

int main(int argc, char** argv) {
  // Set up configuration variables
  std::filesystem::path dateTimeFile;      // date and time
  std::filesystem::path timeStepFile;      // time between logs
  std::filesystem::path numStepsFile;      // time step count
  std::filesystem::path satelliteFile;     // satellite file
  std::filesystem::path sensorSatFile;     // sensor file
  std::filesystem::path txSatFile;         // satellite transmitter file
  std::filesystem::path groundStationFile; // ground station files
  std::filesystem::path rxGndFile;         // gnd station RX files
  std::filesystem::path logDirectory;      // logs destination
  // Parse command line arguments
  if(argc!=3) {
    std::cout << "Usage: " << argv[0]
              << " /path/to/configuration/"
              << " /path/to/logs/"
              << std::endl;
    std::exit(EXIT_SUCCESS);
  } else {
     // Populate configuration variables
    std::filesystem::path configurationDir(argv[1]);
    std::filesystem::directory_iterator it(configurationDir);
    while(it!=std::filesystem::end(it)) {
      std::string pathStr = it->path().string();
      if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-13),13)==
       "date-time.dat"
      ) {
        dateTimeFile = it->path();
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-13),13)==
       "time-step.dat"
      ) {
        timeStepFile = it->path();
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-13),13)==
       "num-steps.dat"
      ) {
        numStepsFile = it->path();
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-4),4)==
       ".sat"
      ) {
        satelliteFile = it->path();
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-25),11)==
       "sensor-sat-"
      ) {
        sensorSatFile = it->path();
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-21),7)==
       "tx-sat-"
      ) {
        txSatFile = it->path();
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-4),4)==
       ".gnd"
      ) {
        groundStationFile = it->path();
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-21),7)==
       "rx-gnd-"
      ) {
        rxGndFile = it->path();
      }
      it++;
    }
    // Set log directory
    logDirectory = std::filesystem::path(argv[2]);
  }
  // Set up log
  std::vector<cote::LogLevel> levels = {cote::LogLevel::INFO};
  cote::Log log(levels,logDirectory.string());
  // Set up date and time
  std::ifstream dateTimeHandle(dateTimeFile.string());
  std::string line = "";
  std::getline(dateTimeHandle,line); // Read header
  std::getline(dateTimeHandle,line); // Read values
  dateTimeHandle.close();
  const int16_t year = static_cast<int16_t>(std::stoi(line.substr(0,4)));
  const uint8_t month = static_cast<uint8_t>(std::stoi(line.substr(5,2)));
  const uint8_t day = static_cast<uint8_t>(std::stoi(line.substr(8,2)));
  const uint8_t hour = static_cast<uint8_t>(std::stoi(line.substr(11,2)));
  const uint8_t minute = static_cast<uint8_t>(std::stoi(line.substr(14,2)));
  const uint8_t second = static_cast<uint8_t>(std::stoi(line.substr(17,2)));
  const uint32_t ns = static_cast<uint32_t>(std::stoul(line.substr(20,9)));
  cote::DateTime dateTime(year,month,day,hour,minute,second,ns);
  // Set up time step variables
  std::ifstream timeStepHandle(timeStepFile.string());
  line = "";
  std::getline(timeStepHandle,line); // Read header
  std::getline(timeStepHandle,line); // Read values
  timeStepHandle.close();
  const uint8_t hourStep = static_cast<uint8_t>(std::stoi(line.substr(0,2)));
  const uint8_t minuteStep = static_cast<uint8_t>(std::stoi(line.substr(3,2)));
  const uint8_t secondStep = static_cast<uint8_t>(std::stoi(line.substr(6,2)));
  const uint32_t nsStep = static_cast<uint32_t>(std::stoul(line.substr(9,9)));
  const double totalStepInSec =
   (
    static_cast<double>(hourStep)*static_cast<double>(cote::cnst::MIN_PER_HOUR)+
    static_cast<double>(minuteStep)
   )*static_cast<double>(cote::cnst::SEC_PER_MIN)+
   static_cast<double>(secondStep)+
   static_cast<double>(nsStep)/static_cast<double>(cote::cnst::NS_PER_SEC);
  // Set up step count
  std::ifstream numStepsHandle(numStepsFile.string());
  line = "";
  std::getline(numStepsHandle,line); // Read header
  std::getline(numStepsHandle,line); // Read values
  numStepsHandle.close();
  const uint64_t numSteps =
   static_cast<uint64_t>(std::stoull(line.substr(0,19)));
  // Set up satellite
  std::string satelliteFileStr = satelliteFile.string();
  std::ifstream satelliteHandle(satelliteFileStr);
  line = "";
  std::getline(satelliteHandle,line); // Read header
  std::getline(satelliteHandle,line); // Read values
  satelliteHandle.close();
  const int16_t satYear = static_cast<int16_t>(std::stoi(line.substr(0,4)));
  const uint8_t satMonth = static_cast<uint8_t>(std::stoi(line.substr(5,2)));
  const uint8_t satDay = static_cast<uint8_t>(std::stoi(line.substr(8,2)));
  const uint8_t satHr = static_cast<uint8_t>(std::stoi(line.substr(11,2)));
  const uint8_t satMin = static_cast<uint8_t>(std::stoi(line.substr(14,2)));
  const uint8_t satSec = static_cast<uint8_t>(std::stoi(line.substr(17,2)));
  const uint32_t satNs = static_cast<uint32_t>(std::stoul(line.substr(20,9)));
  const uint32_t satId = static_cast<uint32_t>(std::stoul(line.substr(30,10)));
  const cote::DateTime satTime(
   satYear,satMonth,satDay,satHr,satMin,satSec,satNs
  );
  cote::Satellite satellite(satelliteFileStr,&dateTime,&log);
  satellite.setID(satId);
  satellite.setLocalTime(satTime);
  std::map<uint32_t,cote::Satellite*> satId2Sat;
  satId2Sat[satId] = &satellite;
  // Set up satellite sensor
  std::ifstream sensorHandle(sensorSatFile.string());
  line = "";
  std::getline(sensorHandle,line); // Read header
  std::getline(sensorHandle,line); // Read value(s)
  sensorHandle.close();
  const uint64_t bitsPerSense =
   static_cast<uint64_t>(std::stoull(line.substr(0,10)));
  const uint32_t pixelCountW =
   static_cast<uint32_t>(std::stoul(line.substr(11,5)));
  const double pixelSizeM =
   static_cast<double>(std::stod(line.substr(17,11)));
  const double focalLengthM =
   static_cast<double>(std::stod(line.substr(29,10)));
  const uint32_t pixelCountH =
   static_cast<uint32_t>(std::stoul(line.substr(40,5)));
  const uint32_t sensorSatId =
   static_cast<uint32_t>(std::stoul(line.substr(46,10)));
  const std::array<double,3> sensorPosn = satId2Sat[sensorSatId]->getECIPosn();
  std::map<uint32_t,const double>   satId2ThreshCoeff;
  std::map<uint32_t,const uint32_t> satId2PixelCountW;
  std::map<uint32_t,const double>   satId2PixelSizeM;
  std::map<uint32_t,const double>   satId2FocalLengthM;
  std::map<uint32_t,const uint32_t> satId2PixelCountH;
  std::map<uint32_t,cote::Sensor*>  satId2Sensor;
  satId2ThreshCoeff.insert(std::make_pair(
   sensorSatId,
   // geographic frame width (km) = threshCoeff*altitude(km)
   static_cast<double>(std::max(pixelCountW,pixelCountH))*pixelSizeM/
   focalLengthM
  ));
  satId2PixelCountW.insert(std::make_pair(sensorSatId,pixelCountW));
  satId2PixelSizeM.insert(std::make_pair(sensorSatId,pixelSizeM));
  satId2FocalLengthM.insert(std::make_pair(sensorSatId,focalLengthM));
  satId2PixelCountH.insert(std::make_pair(sensorSatId,pixelCountH));
  satId2Sensor[sensorSatId] =
   new cote::Sensor(sensorPosn,&dateTime,sensorSatId,&log);
  satId2Sensor[sensorSatId]->setBitsPerSense(bitsPerSense);
  // Set up satellite TX
  std::ifstream txSatHandle(txSatFile.string());
  line = "";
  std::getline(txSatHandle,line); // Read header
  std::getline(txSatHandle,line); // Read values
  txSatHandle.close();
  const uint32_t txSatId = static_cast<uint32_t>(std::stoul(line.substr(0,10)));
  const double txSatPowerW = std::stod(line.substr(11,16));
  const double txSatLineLossDB = std::stod(line.substr(28,13));
  const double txSatGainDB = std::stod(line.substr(42,13));
  const double txSatCenterFrequencyHz = std::stod(line.substr(56,22));
  const double txSatBandwidthHz = std::stod(line.substr(79,23));
  const std::array<double,3> txSatPosn = satId2Sat[txSatId]->getECIPosn();
  std::map<uint32_t,const double> satId2TxCenterFrequencyHz;
  std::map<uint32_t,const double> satId2TxBandwidthHz;
  std::map<uint32_t,cote::Transmitter*> satId2Tx;
  satId2TxCenterFrequencyHz.insert(std::make_pair(
   txSatId,txSatCenterFrequencyHz
  ));
  satId2TxBandwidthHz.insert(std::make_pair(txSatId,txSatBandwidthHz));
  satId2Tx[txSatId] = new cote::Transmitter(
   txSatPosn,txSatPowerW,txSatLineLossDB,txSatGainDB,&dateTime,txSatId,&log
  );
  // Set up ground station
  std::ifstream gndHandle(groundStationFile.string());
  line = "";
  std::getline(gndHandle,line); // Read header
  std::getline(gndHandle,line); // Read values
  gndHandle.close();
  uint32_t gndId = static_cast<uint32_t>(std::stoul(line.substr(0,10)));
  double lat = std::stod(line.substr(11,13));
  double lon = std::stod(line.substr(25,14));
  double hae = std::stod(line.substr(40,13));
  cote::GroundStation groundStation(lat,lon,hae,&dateTime,gndId,&log);
  std::map<uint32_t,cote::GroundStation*> gndId2Gnd;
  gndId2Gnd[gndId] = &groundStation;
  // Set up ground station RX
  std::ifstream rxGndHandle(rxGndFile.string());
  line = "";
  std::getline(rxGndHandle,line); // Read header
  std::getline(rxGndHandle,line); // Read values
  rxGndHandle.close();
  const uint32_t rxGndId = static_cast<uint32_t>(std::stoul(line.substr(0,10)));
  const double rxGndGainDB = std::stod(line.substr(11,13));
  const double rxGndCenterFrequencyHz = std::stod(line.substr(25,22));
  const double rxGndBandwidthHz = std::stod(line.substr(48,23));
  const std::array<double,3> rxGndPosn = gndId2Gnd[rxGndId]->getECIPosn();
  std::map<uint32_t,const double> gndId2RxCenterFrequencyHz;
  std::map<uint32_t,const double> gndId2RxBandwidthHz;
  std::map<uint32_t,cote::Receiver*> gndId2Rx;
  gndId2RxCenterFrequencyHz.insert(std::make_pair(
   rxGndId,rxGndCenterFrequencyHz
  ));
  gndId2RxBandwidthHz.insert(std::make_pair(rxGndId,rxGndBandwidthHz));
  gndId2Rx[rxGndId] = new cote::Receiver(
   rxGndPosn,rxGndGainDB,&dateTime,rxGndId,&log
  );
  // Simulation data
  std::map<uint32_t,double> satId2ThresholdKm;
  //std::map<uint32_t,bool> satId2TxOccupied;
  std::map<uint32_t,uint64_t> satId2TxBufferBits;
  satId2ThresholdKm[satId] =
   satId2ThreshCoeff[satId]*
   cote::util::calcAltitudeKm(satellite.getECIPosn());
  //satId2TxOccupied[satId] = false;
  satId2TxBufferBits[satId] = 0;
  // Simulation loop
  uint64_t stepCount = 0;
  while(stepCount<numSteps) {
    // Date and time
    const double JD = cote::util::calcJulianDayFromYMD(
     dateTime.getYear(),dateTime.getMonth(),dateTime.getDay()
    );
    const uint32_t SEC = cote::util::calcSecSinceMidnight(
     dateTime.getHour(), dateTime.getMinute(), dateTime.getSecond()
    );
    const uint32_t NS = dateTime.getNanosecond();
    // Simulate satellite
    const std::array<double,3> SAT_ECI_POSN_KM = satId2Sat[satId]->getECIPosn();
    // Simulate satellite sensor (data collection)
    const std::array<double,3> PREV_SENSE_POSN =
     satId2Sensor[satId]->getPrevSensePosn();
    const cote::DateTime PREV_SENSE_DATE_TIME =
     satId2Sensor[satId]->getPrevSenseDateTime();
    const double PREV_SENSE_JD = cote::util::calcJulianDayFromYMD(
     PREV_SENSE_DATE_TIME.getYear(), PREV_SENSE_DATE_TIME.getMonth(),
     PREV_SENSE_DATE_TIME.getDay()
    );
    const uint32_t PREV_SENSE_SEC = cote::util::calcSecSinceMidnight(
     PREV_SENSE_DATE_TIME.getHour(), PREV_SENSE_DATE_TIME.getMinute(),
     PREV_SENSE_DATE_TIME.getSecond()
    );
    const uint32_t PREV_SENSE_NS = PREV_SENSE_DATE_TIME.getNanosecond();
    const double PREV_SENSE_LAT =
     cote::util::calcSubpointLatitude(PREV_SENSE_POSN);
    const double PREV_SENSE_LON = cote::util::calcSubpointLongitude(
     PREV_SENSE_JD, PREV_SENSE_SEC, PREV_SENSE_NS, PREV_SENSE_POSN
    );
    const double SAT_LAT =
     cote::util::calcSubpointLatitude(SAT_ECI_POSN_KM);
    const double SAT_LON = cote::util::calcSubpointLongitude(
     JD, SEC, NS, SAT_ECI_POSN_KM
    );
    const double DIST_KM = cote::util::calcGreatCircleArc(
     SAT_LON, SAT_LAT, PREV_SENSE_LON, PREV_SENSE_LAT
    )*cote::cnst::WGS_84_A; // Earth "radius" in km
    if(DIST_KM>=satId2ThresholdKm[satId]) {
      // Increment bits sensed
      satId2TxBufferBits[satId] += satId2Sensor[satId]->getBitsPerSense();
      // Update the threshold value for next GTFR sense event
      const double SAT_ALT_KM = cote::util::calcAltitudeKm(SAT_ECI_POSN_KM);
      satId2ThresholdKm[satId] = satId2ThreshCoeff[satId]*SAT_ALT_KM;
      // Trigger a sense event now to update prevSense values
      satId2Sensor[satId]->triggerSense();
      // Calculate GSD
      const double GSD =
       satId2PixelSizeM[satId]*SAT_ALT_KM*cote::cnst::M_PER_KM/
       satId2FocalLengthM[satId];
      // Log the sense trigger event
      std::ostringstream oss;
      oss << "sat-" << std::setw(10) << std::setfill('0') << satId;
      log.meas(
       cote::LogLevel::INFO,
       //dateTime.toString(),
       std::to_string(stepCount),
       std::string(oss.str()+"-alt-km"),
       std::to_string(SAT_ALT_KM)
      );
      log.meas(
       cote::LogLevel::INFO,
       //dateTime.toString(),
       std::to_string(stepCount),
       std::string(oss.str()+"-x-km"),
       std::to_string(SAT_ECI_POSN_KM.at(0))
      );
      log.meas(
       cote::LogLevel::INFO,
       //dateTime.toString(),
       std::to_string(stepCount),
       std::string(oss.str()+"-y-km"),
       std::to_string(SAT_ECI_POSN_KM.at(1))
      );
      log.meas(
       cote::LogLevel::INFO,
       //dateTime.toString(),
       std::to_string(stepCount),
       std::string(oss.str()+"-z-km"),
       std::to_string(SAT_ECI_POSN_KM.at(2))
      );
      log.meas(
       cote::LogLevel::INFO,
       //dateTime.toString(),
       std::to_string(stepCount),
       std::string(oss.str()+"-lat"),
       std::to_string(SAT_LAT)
      );
      log.meas(
       cote::LogLevel::INFO,
       //dateTime.toString(),
       std::to_string(stepCount),
       std::string(oss.str()+"-lon"),
       std::to_string(SAT_LON)
      );
      log.meas(
       cote::LogLevel::INFO,
       //dateTime.toString(),
       std::to_string(stepCount),
       std::string(oss.str()+"-gsd"),
       std::to_string(GSD)
      );
      log.meas(
       cote::LogLevel::INFO,
       //dateTime.toString(),
       std::to_string(stepCount),
       std::string(oss.str()+"-bits"),
       std::to_string(satId2TxBufferBits[satId])
      );
    }
    // Calculate and log downlink statistics if applicable
    const double GND_LAT = groundStation.getLatitude();
    const double GND_LON = groundStation.getLongitude();
    const double GND_HAE = groundStation.getHAE();
    if(cote::util::calcElevationDeg(
     JD, SEC, NS, GND_LAT, GND_LON, GND_HAE, SAT_ECI_POSN_KM
    )>=10.0) {
      cote::Channel downlink(
       satId2Tx[satId],gndId2Rx[gndId],
       satId2TxCenterFrequencyHz[satId],satId2TxBandwidthHz[satId],
       &dateTime,&log
      );
      const uint64_t TX_BITS = static_cast<uint64_t>(std::round(
       static_cast<double>(downlink.getBitsPerSec())*totalStepInSec
      ));
      satId2Sensor[satId]->drainBuffer(TX_BITS);
      if(TX_BITS<=satId2TxBufferBits[satId]) {
        satId2TxBufferBits[satId] -= TX_BITS;
      } else {
        satId2TxBufferBits[satId] = 0;
      }
      const double downlinkMbps =
       static_cast<double>(downlink.getBitsPerSec())/1.0e6;
      // Assumes one millisecond time step
      if(stepCount%1000==0) {
        std::ostringstream oss;
        oss << "sat-" << std::setw(10) << std::setfill('0') << satId;
        log.meas(
         cote::LogLevel::INFO,
         //dateTime.toString(),
         std::to_string(stepCount),
         std::string(oss.str()+"-bits"),
         std::to_string(satId2TxBufferBits[satId])
        );
        log.meas(
         cote::LogLevel::INFO,
         //dateTime.toString(),
         std::to_string(stepCount),
         std::string(oss.str()+"-downlink-Mbps"),
         std::to_string(downlinkMbps)
        );
      }
    }
    // Update simulation to the next time step
    dateTime.update(hourStep,minuteStep,secondStep,nsStep);
    satellite.update(hourStep,minuteStep,secondStep,nsStep);
    satId2Sensor[satId]->setECIPosn(satellite.getECIPosn());
    satId2Sensor[satId]->update(hourStep,minuteStep,secondStep,nsStep);
    satId2Tx[satId]->setPosn(satellite.getECIPosn());
    satId2Tx[satId]->update(hourStep,minuteStep,secondStep,nsStep);
    groundStation.update(hourStep,minuteStep,secondStep,nsStep);
    gndId2Rx[gndId]->setPosn(groundStation.getECIPosn());
    gndId2Rx[gndId]->update(hourStep,minuteStep,secondStep,nsStep);
    // Increment step count
    stepCount+=1;
  }
  // Write out any remaining logs
  log.writeAll();
  // Clean up ground station RX
  for(
   std::map<uint32_t,cote::Receiver*>::iterator it = gndId2Rx.begin();
   it!=gndId2Rx.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up satellite TX
  for(
   std::map<uint32_t,cote::Transmitter*>::iterator it = satId2Tx.begin();
   it!=satId2Tx.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up sensors
  for(
   std::map<uint32_t,cote::Sensor*>::iterator it = satId2Sensor.begin();
   it!=satId2Sensor.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Exit program
  std::exit(EXIT_SUCCESS);
}
