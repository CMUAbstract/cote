// test-satellite.cpp
// Satellite class test file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <array>         // array
#include <cstddef>       // size_t
#include <cstdlib>       // exit, EXIT_SUCCESS
#include <string>        // to_string

// cote library
#include <DateTime.hpp>  // DateTime
#include <Log.hpp>       // Log
#include <LogLevel.hpp>  // LogLevel
#include <Satellite.hpp> // Satellite

int main(int argc, char** argv) {
  std::vector<cote::LogLevel> levels = {cote::LogLevel::INFO};
  cote::Log log(levels,"../test");
  cote::DateTime dateTime(1980,10,01,23,41,24,113759999);
  cote::Satellite satellite("../data/test.tle",&dateTime,&log);
  for(size_t i=0; i<5; i++) {
    std::array<double,3> eciPosn = satellite.getECIPosn();
    log.meas(
     cote::LogLevel::INFO, dateTime.toString(), "x-km",
     std::to_string(eciPosn.at(0))
    );
    log.meas(
     cote::LogLevel::INFO, dateTime.toString(), "y-km",
     std::to_string(eciPosn.at(1))
    );
    log.meas(
     cote::LogLevel::INFO, dateTime.toString(), "z-km",
     std::to_string(eciPosn.at(2))
    );
    dateTime.update(6,0,0,0);
    satellite.update(6,0,0,0);
  }
  // Write out logs
  log.writeAll();
  std::exit(EXIT_SUCCESS);
}
