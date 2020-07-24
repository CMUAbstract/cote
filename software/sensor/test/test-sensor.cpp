// test-sensor.cpp
// Sensor class test file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <array>        // array
#include <cstddef>      // size_t
#include <cstdlib>      // exit, EXIT_SUCCESS
#include <string>       // to_string

// satsim
#include <DateTime.hpp> // DateTime
#include <Log.hpp>      // Log
#include <LogLevel.hpp> // LogLevel
#include <Sensor.hpp>   // Sensor

int main(int argc, char** argv) {
  std::vector<cote::LogLevel> levels = {cote::LogLevel::INFO};
  cote::Log log(levels,"../test");
  cote::DateTime globalTime(1980,10,01,23,41,24,113759999);
  std::array<double,3> defaultEciPosn = {0.0,0.0,0.0};
  cote::Sensor sensor(defaultEciPosn,&globalTime,0,&log);
  sensor.setBitsPerSense(40000000);
  for(size_t i=0; i<100; i++) {
    std::array<double,3> eciPosn = {static_cast<double>(i),0.0,0.0};
    sensor.setECIPosn(eciPosn);
    if(i%10==0) {
      sensor.triggerSense();
    }
    globalTime.update(1,0,0,0);
    sensor.update(1,0,0,0);
    log.meas(
     cote::LogLevel::INFO, globalTime.toString(), "buffer-MB",
     std::to_string(sensor.getBitsBuffered()/8.0/1.0e6)
    );
  }
  sensor.drainBuffer(80000000);
  log.meas(
   cote::LogLevel::INFO, globalTime.toString(), "buffer-MB",
   std::to_string(sensor.getBitsBuffered()/8.0/1.0e6)
  );
  // Write out logs
  log.writeAll();
  std::exit(EXIT_SUCCESS);
}
