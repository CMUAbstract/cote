// generate-ground-tracks.cpp
// Generates ground track logs based on the configuration files
//
// Usage: ./generate_ground_tracks /path/to/configuration/ /path/to/logs/
// Expected configuration files in /path/to/configuration/:
//  - date-time.dat: The start of the simulation epoch
//  - time-step.dat: The amount of time between each ECI log entry
//  - num-steps.dat: The number of time steps to output to the log
//  - *.tle: A TLE file for each satellite
// /path/to/logs/: The destination for program logs
// Output:
//  - CSV logs with ground station LAT and LON positions
//
// Written by Bradley Denby
// Other contributors: None
//
// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication with this
// software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

// Standard library
#include <algorithm>     // sort
#include <cstdlib>       // exit, EXIT_SUCCESS
#include <filesystem>    // path
#include <fstream>       // ifstream
#include <iomanip>       // setw, setfill
#include <iostream>      // cout
#include <ostream>       // endl
#include <sstream>       // ostringstream
#include <vector>        // vector

// cote library
#include <constants.hpp> // SEC_PER_MIN, MIN_PER_HOUR
#include <DateTime.hpp>  // DateTime
#include <Log.hpp>       // Log
#include <LogLevel.hpp>  // LogLevel
#include <Satellite.hpp> // Satellite
#include <utilities.hpp> // calcJulianDayFromYMD, calcSubpoint Lat & Lon

int main(int argc, char** argv) {
  // Set up variables
  std::filesystem::path dateTimeFile;                // date and time
  std::filesystem::path timeStepFile;                // time between logs
  std::filesystem::path numStepsFile;                // time step count
  std::vector<std::filesystem::path> satelliteFiles; // satellite files
  std::filesystem::path logDirectory;                // logs destination
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
      } else if(pathStr.substr(pathStr.size()-4,4)==".tle") {
        satelliteFiles.push_back(it->path());
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
  // Set up satellites
  std::vector<cote::Satellite> satellites;
  for(size_t i=0; i<satelliteFiles.size(); i++) {
    satellites.push_back(
     cote::Satellite(satelliteFiles.at(i).string(),&dateTime,&log)
    );
  }
  std::sort(
   satellites.begin(), satellites.end(),
   [](const cote::Satellite& s1, const cote::Satellite& s2) {
    return s1.getID() < s2.getID();
   }
  );
  // Simulation loop
  uint64_t stepCount = 0;
  while(stepCount<numSteps) {
    // Update date and time values
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
    // Log satellite ground tracks
    for(size_t i=0; i<satellites.size(); i++) {
      std::ostringstream oss;
      oss << "sat-"
          << std::setw(10) << std::setfill('0') << satellites.at(i).getID()
          << "-";
      log.meas(
       cote::LogLevel::INFO,
       dateTime.toString(),
       oss.str()+"lat-deg",
       std::to_string(
        cote::util::calcSubpointLatitude(satellites.at(i).getECIPosn())
       )
      );
      log.meas(
       cote::LogLevel::INFO,
       dateTime.toString(),
       oss.str()+"lon-deg",
       std::to_string(cote::util::calcSubpointLongitude(
        JD,SEC,NS,satellites.at(i).getECIPosn()
       ))
      );
    }
    // Update simulation to the next time step
    dateTime.update(hourStep,minuteStep,secondStep,nanosecondStep);
    for(size_t i=0; i<satellites.size(); i++) {
      satellites.at(i).update(hourStep,minuteStep,secondStep,nanosecondStep);
    }
    stepCount+=1;
  }
  // Write out logs
  log.writeAll();
  // Clean up
  std::exit(EXIT_SUCCESS);
}
