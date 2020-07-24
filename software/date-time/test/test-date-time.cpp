// test-date-time.cpp
// DateTime class test file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstddef>       // size_t
#include <vector>        // vector

// cote library
#include <DateTime.hpp>  // DateTime
#include <Log.hpp>       // Log
#include <LogLevel.hpp>  // LogLevel

int main(int argc, char** argv) {
  // Setup
  std::vector<cote::LogLevel> levels = {cote::LogLevel::TRACE};
  cote::Log log(levels,"../test");
  // J2000
  cote::DateTime j2000(2000, 1, 1,12, 0, 0,0);
  log.evnt(cote::LogLevel::TRACE,j2000.toString(),"J2000");
  // Write logs
  log.writeAll();
  return 0;
}
