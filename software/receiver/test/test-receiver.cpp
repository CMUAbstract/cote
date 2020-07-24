// test-receiver.cpp
// Receiver class test file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstdlib>      // exit, EXIT_SUCCESS

// cote
#include <DateTime.hpp> // DateTime
#include <Log.hpp>      // Log
#include <LogLevel.hpp> // LogLevel
#include <Receiver.hpp> // Receiver

int main(int argc, char** argv) {
  std::vector<cote::LogLevel> levels = {cote::LogLevel::TRACE};
  cote::Log log(levels,"../test");
  cote::DateTime dateTime(1995,11,18,12,46, 0,0);
  std::array<double,3> posn = {-4400.594, 1932.870, 4760.712};
  cote::Receiver receiver(posn, 6.0, &dateTime, 1, &log);
  log.evnt(cote::LogLevel::TRACE, "Constructor", "test-success");
  std::array<double,3> testPosn = receiver.getPosn();
  log.evnt(cote::LogLevel::TRACE, "getPosn", "test-success");
  double testMaxGainFactor = receiver.getMaxGain();
  log.meas(
   cote::LogLevel::TRACE, dateTime.toString(), "max-gain",
   std::to_string(testMaxGainFactor)
  ); // expected value: 3.981
  log.writeAll();
  std::exit(EXIT_SUCCESS);
}
