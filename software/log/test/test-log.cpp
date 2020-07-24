// test-log.cpp
// Log class test file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstddef>      // size_t
#include <string>       // to_string

// cote library
#include <Log.hpp>      // Log
#include <LogLevel.hpp> // LogLevel

int main(int argc, char** argv) {
  std::vector<cote::LogLevel> levels =
   {cote::LogLevel::INFO,cote::LogLevel::ERROR};
  cote::Log log(levels,"../test");
  std::string dateTime = "2019-06-18T11:17:";
  for(size_t i=0; i<60; i++) {
    log.meas(
     cote::LogLevel::INFO, dateTime+std::to_string(i)+".0",
     "voltage-V", std::to_string(0.0024*static_cast<double>(i))
    );
    log.meas(
     cote::LogLevel::TRACE, dateTime+std::to_string(i)+".0",
     "current-A", std::to_string(0.0002*static_cast<double>(i))
    );
    log.evnt(
     cote::LogLevel::INFO, dateTime+std::to_string(i)+".0", "tick"
    );
  }
  log.writeAll();
  return 0;
}
