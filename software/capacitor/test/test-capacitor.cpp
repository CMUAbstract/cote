// test-capacitor.cpp
// Capacitor class test file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstddef>       // size_t
#include <cstdlib>       // exit, EXIT_SUCCESS
#include <string>        // to_string

// satsim
#include <Capacitor.hpp> // Capacitor
#include <Log.hpp>       // Log
#include <LogLevel.hpp>  // LogLevel

int main(int argc, char** argv) {
  std::vector<cote::LogLevel> levels = {cote::LogLevel::INFO};
  cote::Log log(levels,"../test");
  cote::Capacitor capacitor(5.0,0.168,1,&log);
  for(size_t i=0; i<100; i++) {
    log.meas(
     cote::LogLevel::INFO, std::to_string(i), "charge-C",
     std::to_string(capacitor.getChargeCoulomb())
    );
    capacitor.setChargeCoulomb(capacitor.getChargeCoulomb()+1.0/100.0);
  }
  log.meas(
   cote::LogLevel::INFO, std::to_string(100), "charge-C",
   std::to_string(capacitor.getChargeCoulomb())
  );
  // Write out logs
  log.writeAll();
  std::exit(EXIT_SUCCESS);
}
