// generate-coms.cpp
// Generates communication logs based on the configuration files
//
// Usage: ./generate_coms /path/to/configuration/ /path/to/logs/
// Expected configuration files in /path/to/configuration/:
//  - date-time.dat: The start of the simulation epoch
//  - time-step.dat: The amount of time between each ECI log entry
//  - num-steps.dat: The number of time steps to output to the log
//  - constellation.dat: Specifies the constellation configuration
//  - satellite.tle: A TLE file used to generate constellations
//  - tx-*.dat: Satellite transmitter configuration file
//  - gnd-*.dat: Ground station configuration files
//  - rx-*.dat: Ground station receiver configuration files
// /path/to/logs/: The destination for program logs
// Output:
//  - CSV logs with ECI positions and downlink bitrates
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <algorithm>         // sort
#include <cstdlib>           // exit, EXIT_SUCCESS
#include <filesystem>        // path
#include <fstream>           // ifstream
#include <iomanip>           // setw, setfill
#include <iostream>          // cout
#include <map>               // map
#include <ostream>           // endl
#include <string>            // string
#include <sstream>           // ostringstream
#include <vector>            // vector

// cote library
#include <Channel.hpp>       // Channel
#include <constants.hpp>     // SEC_PER_MIN, MIN_PER_HOUR
#include <DateTime.hpp>      // DateTime
#include <GroundStation.hpp> // GroundStation
#include <Log.hpp>           // Log
#include <LogLevel.hpp>      // LogLevel
#include <Receiver.hpp>      // Receiver
#include <Satellite.hpp>     // Satellite
#include <Transmitter.hpp>   // Transmitter
#include <utilities.hpp>     // calcJulianDayFromYMD

int main(int argc, char** argv) {
  // Set up variables
  std::filesystem::path dateTimeFile;      // date and time
  std::filesystem::path timeStepFile;      // time between logs
  std::filesystem::path numStepsFile;      // time step count
  std::filesystem::path constellationFile; // constellation configuration
  std::filesystem::path satelliteFile;     // satellite file
  std::filesystem::path txSatFile;         // satellite transmitter file
  std::vector<std::filesystem::path> groundStationFiles; // ground station files
  std::vector<std::filesystem::path> rxGndFiles;         // gnd station RX files
  std::filesystem::path logDirectory;      // logs destination
  // Parse command line argument(s)
  if(argc!=3) {
    std::cout << "Usage: " << argv[0]
              << " /path/to/configuration/"
              << " /path/to/logs/"
              << std::endl;
    std::exit(EXIT_SUCCESS);
  } else {
    // Set configuration file values
    std::filesystem::path configurationDir(argv[1]);
    std::filesystem::directory_iterator it;
    it = std::filesystem::directory_iterator(configurationDir);
    while(it!=std::filesystem::end(it)) {
      std::string pathStr = it->path().string();
      if(pathStr.substr(pathStr.size()-13,13)=="date-time.dat") {
        dateTimeFile = it->path();
      } else if(pathStr.substr(pathStr.size()-13,13)=="time-step.dat") {
        timeStepFile = it->path();
      } else if(pathStr.substr(pathStr.size()-13,13)=="num-steps.dat") {
        numStepsFile = it->path();
      } else if(pathStr.substr(pathStr.size()-17,17)=="constellation.dat") {
        constellationFile = it->path();
      } else if(pathStr.substr(pathStr.size()-4,4)==".tle") {
        satelliteFile = it->path();
      } else if(pathStr.substr(pathStr.size()-17,3)=="tx-") {
        txSatFile = it->path();
      } else if(pathStr.substr(pathStr.size()-18,4)=="gnd-") {
        groundStationFiles.push_back(it->path());
      } else if(pathStr.substr(pathStr.size()-17,3)=="rx-") {
        rxGndFiles.push_back(it->path());
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
  int16_t year = static_cast<int16_t>(std::stoi(line.substr(0,4)));
  uint8_t month = static_cast<uint8_t>(std::stoi(line.substr(5,2)));
  uint8_t day = static_cast<uint8_t>(std::stoi(line.substr(8,2)));
  uint8_t hour = static_cast<uint8_t>(std::stoi(line.substr(11,2)));
  uint8_t minute = static_cast<uint8_t>(std::stoi(line.substr(14,2)));
  uint8_t second = static_cast<uint8_t>(std::stoi(line.substr(17,2)));
  uint32_t nanosecond = static_cast<uint32_t>(std::stoi(line.substr(20,9)));
  cote::DateTime dateTime(year,month,day,hour,minute,second,nanosecond);
  // Set up time step variables
  std::ifstream timeStepHandle(timeStepFile.string());
  line = "";
  std::getline(timeStepHandle,line); // Read header
  std::getline(timeStepHandle,line); // Read values
  timeStepHandle.close();
  uint8_t hourStep = static_cast<uint8_t>(std::stoi(line.substr(0,2)));
  uint8_t minuteStep = static_cast<uint8_t>(std::stoi(line.substr(3,2)));
  uint8_t secondStep = static_cast<uint8_t>(std::stoi(line.substr(6,2)));
  uint32_t nanosecondStep = static_cast<uint32_t>(std::stoi(line.substr(9,9)));
  // Set up step count
  std::ifstream numStepsHandle(numStepsFile.string());
  line = "";
  std::getline(numStepsHandle,line); // Read header
  std::getline(numStepsHandle,line); // Read values
  numStepsHandle.close();
  uint64_t numSteps = static_cast<uint64_t>(std::stoi(line.substr(0,19)));
  // Set up constellation configuration parameters
  std::ifstream constellationHandle(constellationFile.string());
  line = "";
  std::getline(constellationHandle,line); // Read header
  std::getline(constellationHandle,line); // Read values
  constellationHandle.close();
  uint16_t cCount = static_cast<uint16_t>(std::stoi(line.substr(0,5)));
  uint8_t cHourStep = static_cast<uint8_t>(std::stoi(line.substr(6,2)));
  uint8_t cMinuteStep = static_cast<uint8_t>(std::stoi(line.substr(9,2)));
  uint8_t cSecondStep = static_cast<uint8_t>(std::stoi(line.substr(12,2)));
  uint32_t cNanosecondStep =
   static_cast<uint32_t>(std::stoi(line.substr(15,9)));
  // Set up satellites
  cote::DateTime localTime = dateTime;
  std::vector<cote::Satellite> satellites;
  for(size_t i=cCount; i!=0; i--) {
    satellites.push_back(
     cote::Satellite(satelliteFile.string(),&dateTime,&log)
    );
    uint32_t id = satellites.back().getID();
    satellites.back().setID(1000*id+(i-1));
    satellites.back().setLocalTime(localTime);
    localTime.update(cHourStep,cMinuteStep,cSecondStep,cNanosecondStep);
  }
  std::sort(
   satellites.begin(), satellites.end(),
   [](const cote::Satellite& s1, const cote::Satellite& s2) {
    return s1.getID() < s2.getID();
   }
  );
  // Set up satellite TX
  double centerFrequencyHz = 0.0;
  double bandwidthHz = 0.0;
  std::map<uint32_t,cote::Transmitter*> satId2Tx;
  for(size_t i=0; i<satellites.size(); i++) {
    std::ifstream satTxHandle(txSatFile.string());
    line = "";
    std::getline(satTxHandle,line); // Read header
    std::getline(satTxHandle,line); // Read values
    satTxHandle.close();
    uint32_t id = satellites.at(i).getID();
    std::array<double,3> posn = satellites.at(i).getECIPosn();
    double powerW = std::stod(line.substr(11,16));
    double lineLossDB = std::stod(line.substr(28,13));
    double gainDB = std::stod(line.substr(42,13));
    centerFrequencyHz = std::stod(line.substr(56,22));
    bandwidthHz = std::stod(line.substr(79,23));
    satId2Tx[id] =
     new cote::Transmitter(posn,powerW,lineLossDB,gainDB,&dateTime,id,&log);
  }
  // Set up ground stations
  std::vector<cote::GroundStation> groundStations;
  for(size_t i=0; i<groundStationFiles.size(); i++) {
    std::ifstream gndHandle(groundStationFiles.at(i).string());
    line = "";
    std::getline(gndHandle,line); // Read header
    std::getline(gndHandle,line); // Read values
    gndHandle.close();
    uint32_t id  = static_cast<uint32_t>(std::stoi(line.substr(0,10)));
    double   lat = std::stod(line.substr(11,13));
    double   lon = std::stod(line.substr(25,14));
    double   hae = std::stod(line.substr(40,13));
    groundStations.push_back(
     cote::GroundStation(lat,lon,hae,&dateTime,id,&log)
    );
  }
  std::sort(
   groundStations.begin(), groundStations.end(),
   [](const cote::GroundStation& g1, const cote::GroundStation& g2) {
    return g1.getID() < g2.getID();
   }
  );
  // Set up ground station RX
  std::map<uint32_t,cote::Receiver*> gndId2Rx;
  for(size_t i=0; i<rxGndFiles.size(); i++) {
    std::ifstream gndRxHandle(rxGndFiles.at(i).string());
    line = "";
    std::getline(gndRxHandle,line); // Read header
    std::getline(gndRxHandle,line); // Read values
    gndRxHandle.close();
    uint32_t id = static_cast<uint32_t>(std::stoi(line.substr(0,10)));
    std::array<double,3> posn = {0.0,0.0,0.0};
    for(size_t i=0; i<groundStations.size(); i++) {
      if(groundStations.at(i).getID()==id) {
        posn = groundStations.at(i).getECIPosn();
      }
    }
    double gainDB = std::stod(line.substr(11,13));
    centerFrequencyHz = std::stod(line.substr(25,22));
    bandwidthHz = std::stod(line.substr(48,23));
    gndId2Rx[id] = new cote::Receiver(posn,gainDB,&dateTime,id,&log);
  }
  // Simulation data
  std::map<uint32_t,bool> satId2Occupied;
  for(size_t i=0; i<satellites.size(); i++) {
    satId2Occupied[satellites.at(i).getID()] = false;
  }
  std::map<uint32_t,std::vector<cote::Satellite*>> gndId2VisSats;
  std::map<uint32_t,cote::Satellite*> gndId2CurrSat;
  for(size_t i=0; i<groundStations.size(); i++) {
    gndId2VisSats[groundStations.at(i).getID()] =
     std::vector<cote::Satellite*>();
    gndId2CurrSat[groundStations.at(i).getID()] = NULL;
  }
  std::vector<cote::Channel> downlinks = std::vector<cote::Channel>();
  // Simulation loop
  uint64_t stepCount = 0;
  while(stepCount<numSteps) {
    // Prepare simulation data
    //// Clear active channels
    downlinks.clear();
    //// Get the current time
    const double JD = cote::util::calcJulianDayFromYMD(
     dateTime.getYear(),dateTime.getMonth(),dateTime.getDay()
    );
    const uint32_t SEC =
     dateTime.getSecond()+
     static_cast<uint32_t>(dateTime.getMinute())*
     static_cast<uint32_t>(cote::cnst::SEC_PER_MIN)+
     static_cast<uint32_t>(dateTime.getHour())*
     static_cast<uint32_t>(cote::cnst::MIN_PER_HOUR)*
     static_cast<uint32_t>(cote::cnst::SEC_PER_MIN);
    const uint32_t NS = dateTime.getNanosecond();
    //// Determinte visible satellites for each ground station
    //// Clear satellite occupied flag if no longer visible
    for(size_t i=0; i<groundStations.size(); i++) {
      const double LAT      = groundStations.at(i).getLatitude();
      const double LON      = groundStations.at(i).getLongitude();
      const double HAE      = groundStations.at(i).getHAE();
      const uint32_t GND_ID = groundStations.at(i).getID();
      gndId2VisSats[GND_ID].clear();
      bool currSatInView  = false;
      for(size_t j=0; j<satellites.size(); j++) {
        const std::array<double,3> satEciPosn = satellites.at(j).getECIPosn();
        if(
         cote::util::calcElevationDeg(JD,SEC,NS,LAT,LON,HAE,satEciPosn)>=10.0
        ) {
          gndId2VisSats[GND_ID].push_back(&(satellites.at(j)));
          if(
           gndId2CurrSat[GND_ID]!=NULL &&
           gndId2CurrSat[GND_ID]==&(satellites.at(j))
          ) {
            currSatInView = true;
          }
        }
      }
      if(!currSatInView && gndId2CurrSat[GND_ID]!=NULL) {
        satId2Occupied[gndId2CurrSat[GND_ID]->getID()] = false;
        gndId2CurrSat[GND_ID] = NULL;
      }
    }
    // Simulation logic
    //// For each ground station without a link, connect with the highest sat
    for(size_t i=0; i<groundStations.size(); i++) {
      const uint32_t GND_ID = groundStations.at(i).getID();
      // If no current link, choose unoccupied visible satellite with most data
      if(gndId2CurrSat[GND_ID]==NULL) {
        const double LAT = groundStations.at(i).getLatitude();
        const double LON = groundStations.at(i).getLongitude();
        const double HAE = groundStations.at(i).getHAE();
        cote::Satellite* bestSat = NULL;
        double highestEl = 0.0;
        for(size_t j=0; j<satellites.size(); j++) {
          const uint32_t SAT_ID = satellites.at(j).getID();
          const std::array<double,3> satEciPosn = satellites.at(j).getECIPosn();
          const double EL =
           cote::util::calcElevationDeg(JD,SEC,NS,LAT,LON,HAE,satEciPosn);
          if(!satId2Occupied[SAT_ID] && EL>=10.0 && EL>highestEl) {
            bestSat = &(satellites.at(j));
            highestEl = EL;
          }
        }
        if(bestSat!=NULL) {
          satId2Occupied[bestSat->getID()] = true;
          gndId2CurrSat[GND_ID] = bestSat;
        }
        bestSat = NULL;
      }
      // If there are links now, formally construct them
      if(gndId2CurrSat[GND_ID]!=NULL) {
        const uint32_t SAT_ID = gndId2CurrSat[GND_ID]->getID();
        downlinks.push_back(
         cote::Channel(
          satId2Tx[SAT_ID],gndId2Rx[GND_ID],centerFrequencyHz,bandwidthHz,
          &dateTime,&log
         )
        );
      }
    }
    // Log info every 100 sim steps
    if(stepCount%100==0) {
      if(downlinks.size()==0) {
        log.meas(
         cote::LogLevel::INFO,
         dateTime.toString(),
         std::string("downlink-Mbps"),
         std::to_string(0.0)
        );
        log.meas(
         cote::LogLevel::INFO,
         dateTime.toString(),
         std::string("downlink-tx-rx"),
         "None"
        );
      } else {
        for(size_t i=0; i<downlinks.size(); i++) {
          log.meas(
           cote::LogLevel::INFO,
           dateTime.toString(),
           std::string("downlink-Mbps"),
           std::to_string(
            static_cast<double>(downlinks.at(i).getBitsPerSec())/1.0e6
           )
          );
          log.meas(
           cote::LogLevel::INFO,
           dateTime.toString(),
           std::string("downlink-tx-rx"),
           std::to_string(downlinks.at(i).getTransmitter()->getID())+"-"+
            std::to_string(downlinks.at(i).getReceiver()->getID())
          );
        }
      }
      for(size_t i=0; i<satellites.size(); i++) {
        std::ostringstream oss;
        oss << "sat-"
            << std::setw(10) << std::setfill('0') << satellites.at(i).getID()
            << "-";
        log.meas(
         cote::LogLevel::INFO,
         dateTime.toString(),
         oss.str()+"eci-x-km",
         std::to_string(satellites.at(i).getECIPosn().at(0))
        );
        log.meas(
         cote::LogLevel::INFO,
         dateTime.toString(),
         oss.str()+"eci-y-km",
         std::to_string(satellites.at(i).getECIPosn().at(1))
        );
        log.meas(
         cote::LogLevel::INFO,
         dateTime.toString(),
         oss.str()+"eci-z-km",
         std::to_string(satellites.at(i).getECIPosn().at(2))
        );
      }
    }
    // Update simulation to the next time step
    dateTime.update(hourStep,minuteStep,secondStep,nanosecondStep);
    for(size_t i=0; i<satellites.size(); i++) {
      satellites.at(i).update(hourStep,minuteStep,secondStep,nanosecondStep);
      satId2Tx[satellites.at(i).getID()]->setPosn(
       satellites.at(i).getECIPosn()
      );
    }
    for(size_t i=0; i<groundStations.size(); i++) {
      groundStations.at(i).update(
       hourStep,minuteStep,secondStep,nanosecondStep
      );
      gndId2Rx[groundStations.at(i).getID()]->setPosn(
       groundStations.at(i).getECIPosn()
      );
    }
    stepCount+=1;
  }
  // Write out logs
  log.writeAll();
  // Clean up
  for(
   std::map<uint32_t,cote::Transmitter*>::iterator it=satId2Tx.begin();
   it!=satId2Tx.end(); it++
  ) {
    delete it->second;
  }
  for(
   std::map<uint32_t,cote::Receiver*>::iterator it=gndId2Rx.begin();
   it!=gndId2Rx.end(); it++
  ) {
    delete it->second;
  }
  // Exit
  std::exit(EXIT_SUCCESS);
}
