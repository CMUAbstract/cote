// generate-ecis.cpp
// Generates ECI orbit coordinates based on the configuration files
//
// Usage: ./generate_ecis /path/to/configuration/ /path/to/logs/
// Expected configuration files in /path/to/configuration/:
//  - date-time.dat: The start of the simulation epoch
//  - *.tle: A TLE file for the satellite in question
//  - time-step.dat: The amount of time between each ECI log entry
//  - num-steps.dat: The number of time steps to output to the log
// /path/to/logs/: The path to the directory for log files
// Output:
//  - CSV logs with the satellite ECI x, y, and z positions at each time step
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

// cote library
#include <DateTime.hpp>      // DateTime
#include <Log.hpp>           // Log
#include <LogLevel.hpp>      // LogLevel
#include <Satellite.hpp>     // Satellite

int main(int argc, char** argv) {
  // Set up variables
  std::filesystem::path dateTimeFile;            // file specifying date & time
  std::filesystem::path satelliteFile;           // satellite TLE file
  std::filesystem::path timeStepFile;            // specifies time between logs
  std::filesystem::path numStepsFile;            // specifies time step count
  std::filesystem::path logDirectory;            // path to log destination
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
      } else if(pathStr.substr(pathStr.size()-4,4)==".tle") {
        satelliteFile = it->path();
      } else if(pathStr.substr(pathStr.size()-13,13)=="time-step.dat") {
        timeStepFile = it->path();
      } else if(pathStr.substr(pathStr.size()-13,13)=="num-steps.dat") {
        numStepsFile = it->path();
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
  // Set up satellite
  cote::DateTime localTime = dateTime;
  cote::Satellite satellite(satelliteFile.string(),&dateTime,&log);
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
  // Simulation loop
  uint64_t stepCount = 0;
  while(stepCount<numSteps) {
    // Log satellite position
    log.meas(
     cote::LogLevel::INFO,
     dateTime.toString(),
     std::string("satellite-x"),
     std::to_string(satellite.getECIPosn().at(0))
    );
    log.meas(
     cote::LogLevel::INFO,
     dateTime.toString(),
     std::string("satellite-y"),
     std::to_string(satellite.getECIPosn().at(1))
    );
    log.meas(
     cote::LogLevel::INFO,
     dateTime.toString(),
     std::string("satellite-z"),
     std::to_string(satellite.getECIPosn().at(2))
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
