// crosslink-los.cpp
//
// Simulates a constellation of satellites and logs crosslink line-of-sight
//
// Requires C++17 (e.g. GCC 8) for std::filesystem
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <algorithm>         // max, sort
#include <cstdlib>           // exit, EXIT_SUCCESS
#include <filesystem>        // path
#include <fstream>           // ifstream
#include <iomanip>           // setw, setfill
#include <iostream>          // cout
#include <ostream>           // endl
#include <string>            // string
#include <sstream>           // ostringstream
#include <vector>            // vector

// cote
#include <DateTime.hpp>      // DateTime
#include <Log.hpp>           // Log
#include <LogLevel.hpp>      // LogLevel
#include <Satellite.hpp>     // Satellite
#include <utilities.hpp>     // various; search util::

int main(int argc, char** argv) {
  // Set up configuration variables
  std::filesystem::path dateTimeFile;                // start date and time
  std::filesystem::path timeStepFile;                // time between steps
  std::filesystem::path numStepsFile;                // time step count
  std::vector<std::filesystem::path> satelliteFiles; // satellite TLEs
  std::filesystem::path logDirectory;                // logs destination
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
  // Set up step count
  std::ifstream numStepsHandle(numStepsFile.string());
  line = "";
  std::getline(numStepsHandle,line); // Read header
  std::getline(numStepsHandle,line); // Read values
  numStepsHandle.close();
  const uint64_t numSteps =
   static_cast<uint64_t>(std::stoull(line.substr(0,19)));
  // Set up satellites
  std::vector<cote::Satellite> satellites;
  for(std::size_t i=0; i<satelliteFiles.size(); i++) {
    std::string satelliteFileStr = satelliteFiles.at(i).string();
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
    const uint32_t id = static_cast<uint32_t>(std::stoul(line.substr(30,10)));
    const cote::DateTime satTime(
     satYear,satMonth,satDay,satHr,satMin,satSec,satNs
    );
    satellites.push_back(cote::Satellite(satelliteFileStr,&dateTime,&log));
    satellites.back().setID(id);
    satellites.back().setLocalTime(satTime);
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
    for(std::size_t i=0; i<satellites.size(); i++) {
      const uint32_t SAT_ID_I = satellites.at(i).getID();
      const std::array<double,3> srcSatEciPosnKm =
       satellites.at(i).getECIPosn();
      // Log every satellite position at every time step
      std::ostringstream oss;
      oss << "sat-" << std::setw(10) << std::setfill('0') << SAT_ID_I;
      log.meas(
       cote::LogLevel::INFO,
       dateTime.toString(),
       std::string(oss.str()+"-x-km"),
       std::to_string(srcSatEciPosnKm.at(0))
      );
      log.meas(
       cote::LogLevel::INFO,
       dateTime.toString(),
       std::string(oss.str()+"-y-km"),
       std::to_string(srcSatEciPosnKm.at(1))
      );
      log.meas(
       cote::LogLevel::INFO,
       dateTime.toString(),
       std::string(oss.str()+"-z-km"),
       std::to_string(srcSatEciPosnKm.at(2))
      );
      for(std::size_t j=0; j<satellites.size(); j++) {
        const uint32_t SAT_ID_J = satellites.at(j).getID();
        if(SAT_ID_I!=SAT_ID_J) {
          // Log whether a crosslink LoS is available for every other satellite
          const std::array<double,3> dstSatEciPosnKm =
           satellites.at(j).getECIPosn();
          if(
           cote::util::crosslinkLoSAvailable(srcSatEciPosnKm,dstSatEciPosnKm)
          ) {
            // crosslink LoS available; log dst sat ID and distance
            log.meas(
             cote::LogLevel::INFO,
             dateTime.toString(),
             std::string(oss.str()+"-crosslink-dst"),
             std::to_string(SAT_ID_J)
            );
            log.meas(
             cote::LogLevel::INFO,
             dateTime.toString(),
             std::string(oss.str()+"-crosslink-km"),
             std::to_string(cote::util::magnitude(
              cote::util::calcSeparationVector(dstSatEciPosnKm,srcSatEciPosnKm)
             ))
            );
          } 
        }
      }
    }
    // Increment step count
    stepCount+=1;
  }
  // Write out any remaining logs
  log.writeAll();
  // Clean up satellites
  //// Nothing to be done
  // Clean up step count
  //// Nothing to be done
  // Clean up time step variables
  //// Nothing to be done
  // Clean up date and time
  //// Nothing to be done
  std::exit(EXIT_SUCCESS);
}
