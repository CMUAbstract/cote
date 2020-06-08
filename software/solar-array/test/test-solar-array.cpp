// test-solar-array.cpp
// SolarArray class test file
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
#include <cstddef>        // size_t
#include <cstdlib>        // exit, EXIT_SUCCESS
#include <string>         // to_string

// satsim
#include <constants.hpp>  // constants
#include <Log.hpp>        // Log
#include <LogLevel.hpp>   // LogLevel
#include <SolarArray.hpp> // SolarArray

int main(int argc, char** argv) {
  std::vector<cote::LogLevel> levels = {cote::LogLevel::INFO};
  cote::Log log(levels,"../test");
  cote::SolarArray solarArray(2.4,0.0016,0.29,1,&log);
  for(size_t i=0; i<100; i++) {
    solarArray.setIrradianceWpM2(
     (static_cast<double>(100-i)/100.0)*cote::cnst::SOLAR_CONSTANT
    );
    log.meas(
     cote::LogLevel::INFO, std::to_string(i), "current-A",
     std::to_string(solarArray.getCurrentAmpere())
    );
  }
  solarArray.setIrradianceWpM2(0.0);
  log.meas(
   cote::LogLevel::INFO, std::to_string(100), "current-A",
   std::to_string(solarArray.getCurrentAmpere())
  );
  // Write out logs
  log.writeAll();
  std::exit(EXIT_SUCCESS);
}
