// test-ground-station.cpp
// GroundStation class test file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <array>             // array
#include <cstdlib>           // exit, EXIT_SUCCESS
#include <string>            // to_string

// satsim
#include <constants.hpp>     // constants
#include <DateTime.hpp>      // DateTime
#include <GroundStation.hpp> // GroundStation
#include <Log.hpp>           // Log
#include <LogLevel.hpp>      // LogLevel
#include <utilities.hpp>     // utilities

int main(int argc, char** argv) {
  // Setup
  std::vector<cote::LogLevel> levels = {cote::LogLevel::INFO};
  cote::Log log(levels,"../logs");
  // Test 01
  cote::DateTime dt1(1995,10, 1, 0, 0, 0,0);
  cote::GroundStation gs1(40.0,-75.0,0.025,&dt1,1,&log);
  const double JD1 = cote::util::calcJulianDayFromYMD(
   dt1.getYear(), dt1.getMonth(), dt1.getDay()
  );
  const uint32_t SEC1 = cote::util::calcSecSinceMidnight(
   dt1.getHour(), dt1.getMinute(), dt1.getSecond()
  );
  const uint32_t NS1 = dt1.getNanosecond();
  log.meas(
   cote::LogLevel::INFO, dt1.toString(), "test-01-zrot", std::to_string(
    cote::util::calcGMSTRadFromUT1(JD1,SEC1,NS1)/cote::cnst::RAD_PER_DEG
   ) // expected value: 9.257436
  );
  // Test 02
  cote::DateTime dt2(1995,10, 1, 9, 0, 0,0);
  cote::GroundStation gs2(40.0,-75.0,0.0,&dt2,2,&log);
  const double JD2 = cote::util::calcJulianDayFromYMD(
   dt2.getYear(), dt2.getMonth(), dt2.getDay()
  );
  const uint32_t SEC2 = cote::util::calcSecSinceMidnight(
   dt2.getHour(), dt2.getMinute(), dt2.getSecond()
  );
  const uint32_t NS2 = dt2.getNanosecond();
  log.meas(
   cote::LogLevel::INFO, dt2.toString(), "test-02-zrot", std::to_string(
    cote::util::calcGMSTRadFromUT1(JD2,SEC2,NS2)/cote::cnst::RAD_PER_DEG
   ) // expected value: 144.627053
  );
  std::array<double,3> gs2Posn = gs2.getECIPosn();
  log.meas(
   cote::LogLevel::INFO, dt2.toString(), "test-02-x-km", std::to_string(gs2Posn.at(0))
  ); // expected value: 1703.295619
  log.meas(
   cote::LogLevel::INFO, dt2.toString(), "test-02-y-km", std::to_string(gs2Posn.at(1))
  ); // expected value: 4586.651469
  log.meas(
   cote::LogLevel::INFO, dt2.toString(), "test-02-z-km", std::to_string(gs2Posn.at(2))
  ); // expected value: 4077.985572
  // Test 03
  cote::DateTime dt3(1995,11,18,12,46, 0,0);
  cote::GroundStation gs3(45.0,-93.0,0.0,&dt3,3,&log);
  const double JD3 = cote::util::calcJulianDayFromYMD(
   dt3.getYear(), dt3.getMonth(), dt3.getDay()
  );
  const uint32_t SEC3 = cote::util::calcSecSinceMidnight(
   dt3.getHour(), dt3.getMinute(), dt3.getSecond()
  );
  const uint32_t NS3 = dt3.getNanosecond();
  std::array<double,3> satPosn = {-4400.594,1932.870,4760.712};
  log.meas(
   cote::LogLevel::INFO, dt3.toString(), "test-03-el-deg",
   std::to_string(cote::util::calcElevationDeg(
    JD3,SEC3,NS3,gs3.getLatitude(),gs3.getLongitude(),gs3.getHAE(),satPosn
   ))
  ); // expected value: 81.518231
  // Write out logs
  log.writeAll();
  std::exit(EXIT_SUCCESS);
}
