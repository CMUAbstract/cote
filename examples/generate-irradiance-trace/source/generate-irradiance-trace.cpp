// generate-irradiance-trace.cpp
//
// Generates an irradiance trace for the indicated satellite over the indicated
// data and times.
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
#include <Log.hpp>           // Log
#include <LogLevel.hpp>      // LogLevel
#include <Satellite.hpp>     // Satellite
#include <utilities.hpp>     // calcJulianDayFromYMD, calcSecSinceMidnight

int main(int argc, char** argv) {
  // Set up variables
  std::filesystem::path dateTimeFile;  // date and time
  std::filesystem::path timeStepFile;  // time between logs
  std::filesystem::path numStepsFile;  // time step count
  std::filesystem::path satelliteFile; // satellite file
  std::filesystem::path logDirectory;  // logs destination
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
      } else if(pathStr.substr(pathStr.size()-4,4)==".tle") {
        satelliteFile = it->path();
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
  int16_t year = static_cast<int16_t>(std::stoll(line.substr(0,4)));
  uint8_t month = static_cast<uint8_t>(std::stoll(line.substr(5,2)));
  uint8_t day = static_cast<uint8_t>(std::stoll(line.substr(8,2)));
  uint8_t hour = static_cast<uint8_t>(std::stoll(line.substr(11,2)));
  uint8_t minute = static_cast<uint8_t>(std::stoll(line.substr(14,2)));
  uint8_t second = static_cast<uint8_t>(std::stoll(line.substr(17,2)));
  uint32_t nanosecond = static_cast<uint32_t>(std::stoll(line.substr(20,9)));
  cote::DateTime dateTime(year,month,day,hour,minute,second,nanosecond);
  // Set up time step variables
  std::ifstream timeStepHandle(timeStepFile.string());
  line = "";
  std::getline(timeStepHandle,line); // Read header
  std::getline(timeStepHandle,line); // Read values
  timeStepHandle.close();
  uint8_t hourStep = static_cast<uint8_t>(std::stoll(line.substr(0,2)));
  uint8_t minuteStep = static_cast<uint8_t>(std::stoll(line.substr(3,2)));
  uint8_t secondStep = static_cast<uint8_t>(std::stoll(line.substr(6,2)));
  uint32_t nanosecondStep = static_cast<uint32_t>(std::stoll(line.substr(9,9)));
  // Set up step count
  std::ifstream numStepsHandle(numStepsFile.string());
  line = "";
  std::getline(numStepsHandle,line); // Read header
  std::getline(numStepsHandle,line); // Read values
  numStepsHandle.close();
  uint64_t numSteps = static_cast<uint64_t>(std::stoll(line.substr(0,19)));
  // Set up satellite
  cote::Satellite satellite(satelliteFile.string(),&dateTime,&log);
  // Simulation loop
  uint64_t stepCount = 0;
  while(stepCount<numSteps) {
    // Prepare simulation data
    const double JD = cote::util::calcJulianDayFromYMD(
     dateTime.getYear(),dateTime.getMonth(),dateTime.getDay()
    );
    const uint32_t SEC = cote::util::calcSecSinceMidnight(
     dateTime.getHour(), dateTime.getMinute(), dateTime.getSecond()
    );
    const uint32_t NS = dateTime.getNanosecond();
    // Simulation logic
    std::array<double,3> sunEciPosnKm = cote::util::calcSunEciPosnKm(JD,SEC,NS);
    std::array<double,3> satEciPosnKm = satellite.getECIPosn();
    const double sunOcclusionFactor =
     cote::util::calcSunOcclusionFactor(satEciPosnKm,sunEciPosnKm);
    //// When sunOcclusionFactor is 0.0, irradiance is SOLAR_CONSTANT
    //// When sunOcclusionFactor is 1.0, irradiance is 1% of SOLAR_CONSTANT
    const double irradianceWPerM2 =
     -1352.44*sunOcclusionFactor+cote::cnst::SOLAR_CONSTANT;
    // Log the results
    log.meas(
     cote::LogLevel::INFO,
     dateTime.toString(),
     std::string("sat-x-km"),
     std::to_string(satEciPosnKm.at(0))
    );
    log.meas(
     cote::LogLevel::INFO,
     dateTime.toString(),
     std::string("sat-y-km"),
     std::to_string(satEciPosnKm.at(1))
    );
    log.meas(
     cote::LogLevel::INFO,
     dateTime.toString(),
     std::string("sat-z-km"),
     std::to_string(satEciPosnKm.at(2))
    );
    log.meas(
     cote::LogLevel::INFO,
     dateTime.toString(),
     std::string("sat-irradiance-w-p-m2"),
     std::to_string(irradianceWPerM2)
    );
    // Update simulation to the next time step
    dateTime.update(hourStep,minuteStep,secondStep,nanosecondStep);
    satellite.update(hourStep,minuteStep,secondStep,nanosecondStep);
    stepCount+=1;
  }
  // Write out logs
  log.writeAll();
  // Clean up
  std::exit(EXIT_SUCCESS);
}
