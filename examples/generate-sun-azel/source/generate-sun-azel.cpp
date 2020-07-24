// generate-sun-azel.cpp
//
// Generates azimuth and elevation values for the sun.
//
// Requires C++17 (e.g. GCC 8) for std::filesystem
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstdlib>           // exit, EXIT_SUCCESS
#include <filesystem>        // path
#include <fstream>           // ifstream
#include <iostream>          // cout
#include <ostream>           // endl
#include <string>            // string
#include <vector>            // vector

// cote
#include <constants.hpp>     // constants
#include <DateTime.hpp>      // DateTime
#include <GroundStation.hpp> // GroundStation
#include <Log.hpp>           // Log
#include <LogLevel.hpp>      // LogLevel
#include <utilities.hpp>     // calcJulianDayFromYMD, calcSecSinceMidnight

int main(int argc, char** argv) {
  // Set up variables
  std::filesystem::path dateTimeFile;      // date and time
  std::filesystem::path timeStepFile;      // time between logs
  std::filesystem::path numStepsFile;      // time step count
  std::filesystem::path groundStationFile; // ground station file
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
    std::filesystem::directory_iterator it(configurationDir);
    while(it!=std::filesystem::end(it)) {
      std::string pathStr = it->path().string();
      if(pathStr.substr(pathStr.size()-13,13)=="date-time.dat") {
        dateTimeFile = it->path();
      } else if(pathStr.substr(pathStr.size()-13,13)=="time-step.dat") {
        timeStepFile = it->path();
      } else if(pathStr.substr(pathStr.size()-13,13)=="num-steps.dat") {
        numStepsFile = it->path();
      } else if(pathStr.substr(pathStr.size()-7,7)=="gnd.dat") {
        groundStationFile = it->path();
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
  double totalStepInSec =
   (
    static_cast<double>(hourStep)*static_cast<double>(cote::cnst::MIN_PER_HOUR)+
    static_cast<double>(minuteStep)
   )*static_cast<double>(cote::cnst::SEC_PER_MIN)+
   static_cast<double>(secondStep)+
   static_cast<double>(nanosecondStep)/
   static_cast<double>(cote::cnst::NS_PER_SEC);
  // Set up step count
  std::ifstream numStepsHandle(numStepsFile.string());
  line = "";
  std::getline(numStepsHandle,line); // Read header
  std::getline(numStepsHandle,line); // Read values
  numStepsHandle.close();
  uint64_t numSteps = static_cast<uint64_t>(std::stoi(line.substr(0,19)));
  // Set up ground station
  std::ifstream gndHandle(groundStationFile.string());
  line = "";
  std::getline(gndHandle,line); // Read header
  std::getline(gndHandle,line); // Read values
  gndHandle.close();
  uint32_t id  = static_cast<uint32_t>(std::stoi(line.substr(0,10)));
  double   lat = std::stod(line.substr(11,13));
  double   lon = std::stod(line.substr(25,14));
  double   hae = std::stod(line.substr(40,13));
  cote::GroundStation groundStation(lat,lon,hae,&dateTime,id,&log);
  // Simulation loop
  uint64_t stepCount = 0;
  while(stepCount<numSteps) {
    // Get the current time
    const double JD = cote::util::calcJulianDayFromYMD(
     dateTime.getYear(),dateTime.getMonth(),dateTime.getDay()
    );
    const uint32_t SEC = cote::util::calcSecSinceMidnight(
     dateTime.getHour(), dateTime.getMinute(), dateTime.getSecond()
    );
    const uint32_t NS = dateTime.getNanosecond();
    // Simulation logic
    std::array<double,3> sunEciPosnKm = cote::util::calcSunEciPosnKm(JD,SEC,NS);
    const double sunAzDeg = cote::util::calcAzimuthDeg(
     JD,SEC,NS,groundStation.getLatitude(),groundStation.getLongitude(),0.0,
     sunEciPosnKm
    );
    const double sunElDeg = cote::util::calcElevationDeg(
     JD,SEC,NS,groundStation.getLatitude(),groundStation.getLongitude(),0.0,
     sunEciPosnKm
    );
    // Log the results
    log.meas(
     cote::LogLevel::INFO,
     dateTime.toString(),
     std::string("sun-azimuth-deg"),
     std::to_string(sunAzDeg)
    );
    log.meas(
     cote::LogLevel::INFO,
     dateTime.toString(),
     std::string("sun-elevation-deg"),
     std::to_string(sunElDeg)
    );
    // Update simulation to the next time step
    dateTime.update(hourStep,minuteStep,secondStep,nanosecondStep);
    groundStation.update(hourStep,minuteStep,secondStep,nanosecondStep);
    stepCount+=1;
  }
  // Write out logs
  log.writeAll();
  // Clean up
  std::exit(EXIT_SUCCESS);
}
