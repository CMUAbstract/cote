// test-channel.cpp
// Channel class test file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstdlib>         // exit, EXIT_SUCCESS

// cote
#include <Channel.hpp>     // Channel
#include <DateTime.hpp>    // DateTime
#include <Log.hpp>         // Log
#include <LogLevel.hpp>    // LogLevel
#include <Receiver.hpp>    // Receiver
#include <Transmitter.hpp> // Transmitter

int main(int argc, char** argv) {
  std::vector<cote::LogLevel> levels = {cote::LogLevel::TRACE};
  cote::Log log(levels,"../test");
  cote::DateTime globalTime(1995,11,18,12,46, 0,0);
  std::array<double,3> posn = {-4400.594, 1932.870, 4760.712};
  cote::Transmitter transmitter(posn, 15.0, -1.0, 6.0, &globalTime, 1, &log);
  cote::Receiver receiver(posn, 6.0, &globalTime, 2, &log);
  cote::Channel channel(&transmitter,&receiver,8.0e9,20.0e6,&globalTime,&log);
  std::exit(EXIT_SUCCESS);
}
