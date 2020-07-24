// generate-deadlines.cpp
//
// Simulates a satellite as it passes over its ground track and logs ground
// track frame observation times.
//
// Requires C++17 (e.g. GCC 8) for std::filesystem
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstdlib>       // exit, EXIT_SUCCESS
#include <filesystem>    // path
#include <fstream>       // ifstream
#include <iomanip>       // setw, setfill
#include <iostream>      // cout
#include <ostream>       // endl
#include <string>        // string
#include <vector>        // vector

// cote
#include <constants.hpp> // constants
#include <DateTime.hpp>  // DateTime
#include <Log.hpp>       // Log
#include <LogLevel.hpp>  // LogLevel
#include <Satellite.hpp> // Satellite
#include <Sensor.hpp>    // Sensor
#include <utilities.hpp> // calcJulianDayFromYMD, calcSecSinceMidnight

int main(int argc, char** argv) {
  // Set up variables
  std::filesystem::path dateTimeFile;  // date and time
  std::filesystem::path timeStepFile;  // time between logs
  std::filesystem::path numStepsFile;  // time step count
  std::filesystem::path satelliteFile; // satellite file
  std::filesystem::path sensorFile;    // sensor file
  std::filesystem::path logDirectory;  // logs destination
  // Parse command line argument(s)
  if(argc!=3) {
    std::cout << "Usage: " << argv[0]
              << " /path/to/configuration/"
              << " /path/to/logs/"
              << std::endl;
    std::exit(EXIT_SUCCESS);
  } else {
    // Set configuration file values
    std::filesystem::path configurationDir(argv[1]);
    std::filesystem::directory_iterator it(configurationDir);
    while(it!=std::filesystem::end(it)) {
      std::string pathStr = it->path().string();
      if(pathStr.substr(pathStr.size()-13,13)=="date-time.dat") {
        dateTimeFile = it->path();
      } else if(pathStr.substr(pathStr.size()-13,13)=="time-step.dat") {
        timeStepFile = it->path();
      } else if(pathStr.substr(pathStr.size()-13,13)=="num-steps.dat") {
        numStepsFile = it->path();
      } else if(pathStr.substr(pathStr.size()-4,4)==".tle") {
        satelliteFile = it->path();
      } else if(pathStr.substr(pathStr.size()-10,10)=="sensor.dat") {
        sensorFile = it->path();
      }
      it++;
    }
    // Set log directory
    logDirectory = std::filesystem::path(argv[2]);
  }
  // Set up log
  const std::vector<cote::LogLevel> levels = {cote::LogLevel::INFO};
  cote::Log log(levels,logDirectory.string());
  // Set up date and time
  std::ifstream dateTimeHandle(dateTimeFile.string());
  std::string line = "";
  std::getline(dateTimeHandle,line); // Read header
  std::getline(dateTimeHandle,line); // Read values
  dateTimeHandle.close();
  const int16_t year = static_cast<int16_t>(std::stoll(line.substr(0,4)));
  const uint8_t month = static_cast<uint8_t>(std::stoll(line.substr(5,2)));
  const uint8_t day = static_cast<uint8_t>(std::stoll(line.substr(8,2)));
  const uint8_t hour = static_cast<uint8_t>(std::stoll(line.substr(11,2)));
  const uint8_t minute = static_cast<uint8_t>(std::stoll(line.substr(14,2)));
  const uint8_t second = static_cast<uint8_t>(std::stoll(line.substr(17,2)));
  const uint32_t nanosecond =
   static_cast<uint32_t>(std::stoll(line.substr(20,9)));
  cote::DateTime dateTime(year,month,day,hour,minute,second,nanosecond);
  // Set up time step variables
  std::ifstream timeStepHandle(timeStepFile.string());
  line = "";
  std::getline(timeStepHandle,line); // Read header
  std::getline(timeStepHandle,line); // Read values
  timeStepHandle.close();
  const uint8_t hourStep = static_cast<uint8_t>(std::stoll(line.substr(0,2)));
  const uint8_t minuteStep = static_cast<uint8_t>(std::stoll(line.substr(3,2)));
  const uint8_t secondStep = static_cast<uint8_t>(std::stoll(line.substr(6,2)));
  const uint32_t nanosecondStep =
   static_cast<uint32_t>(std::stoll(line.substr(9,9)));
  // Set up step count
  std::ifstream numStepsHandle(numStepsFile.string());
  line = "";
  std::getline(numStepsHandle,line); // Read header
  std::getline(numStepsHandle,line); // Read values
  numStepsHandle.close();
  const uint64_t numSteps =
   static_cast<uint64_t>(std::stoll(line.substr(0,19)));
  // Set up satellite
  cote::DateTime localTime = dateTime;
  cote::Satellite satellite(satelliteFile.string(),&dateTime,&log);
  satellite.setLocalTime(localTime);
  const uint32_t SAT_ID = satellite.getID();
  // Set up satellite sensor
  std::ifstream sensorHandle(sensorFile.string());
  line = "";
  std::getline(sensorHandle,line); // Read header
  std::getline(sensorHandle,line); // Read value(s)
  sensorHandle.close();
  const uint64_t bitsPerSense =
   static_cast<uint64_t>(std::stoll(line.substr(0,10)));
  const uint32_t pixelCount =
   static_cast<uint32_t>(std::stoll(line.substr(11,5)));
  const double pixelSizeM = static_cast<double>(std::stod(line.substr(17,11)));
  const double focalLengthM =
   static_cast<double>(std::stod(line.substr(29,10)));
  const double threshCoeff = // frame width (km) = threshCoeff*altitude(km)
   (static_cast<double>(pixelCount)*pixelSizeM/focalLengthM);
  std::map<uint32_t,cote::Sensor*> satId2Sensor;
  satId2Sensor[SAT_ID] =
   new cote::Sensor(satellite.getECIPosn(),&dateTime,SAT_ID,&log);
  satId2Sensor[SAT_ID]->setBitsPerSense(bitsPerSense);
  // Simulation data
  std::map<uint32_t,double> satId2ThresholdKm;
  satId2ThresholdKm[SAT_ID] =
   threshCoeff*cote::util::calcAltitudeKm(satellite.getECIPosn());
  // Simulation loop
  uint64_t stepCount = 0;
  while(stepCount<numSteps) {
    // Prepare simulation data
    //// Get the current time
    const double JD = cote::util::calcJulianDayFromYMD(
     dateTime.getYear(),dateTime.getMonth(),dateTime.getDay()
    );
    const uint32_t SEC = cote::util::calcSecSinceMidnight(
     dateTime.getHour(), dateTime.getMinute(), dateTime.getSecond()
    );
    const uint32_t NS = dateTime.getNanosecond();
    // Simulation logic
    //// Sensor data collection logic
    const std::array<double,3> prevSensePosn =
     satId2Sensor[SAT_ID]->getPrevSensePosn();
    const cote::DateTime prevSenseDateTime =
     satId2Sensor[SAT_ID]->getPrevSenseDateTime();
    const double PREV_JD = cote::util::calcJulianDayFromYMD(
     prevSenseDateTime.getYear(), prevSenseDateTime.getMonth(),
     prevSenseDateTime.getDay()
    );
    const uint32_t PREV_SEC = cote::util::calcSecSinceMidnight(
     prevSenseDateTime.getHour(), prevSenseDateTime.getMinute(),
     prevSenseDateTime.getSecond()
    );
    const uint32_t PREV_NS = prevSenseDateTime.getNanosecond();
    const double PREV_LAT = cote::util::calcSubpointLatitude(prevSensePosn);
    const double PREV_LON = cote::util::calcSubpointLongitude(
     PREV_JD, PREV_SEC, PREV_NS, prevSensePosn
    );
    const std::array<double,3> currPosn = satId2Sensor[SAT_ID]->getECIPosn();
    const double CURR_LAT = cote::util::calcSubpointLatitude(currPosn);
    const double CURR_LON = cote::util::calcSubpointLongitude(
     JD, SEC, NS, currPosn
    );
    const double distanceKm = cote::util::calcGreatCircleArc(
     CURR_LON, CURR_LAT, PREV_LON, PREV_LAT
    )*cote::cnst::WGS_84_A; // Earth "radius" in km
    if(distanceKm>=satId2ThresholdKm[SAT_ID]) {
      const std::array<double,3> satEciPosn = satellite.getECIPosn();
      std::ostringstream oss;
      oss << "sat-" << std::setw(5) << std::setfill('0') << SAT_ID;
      log.meas(
       cote::LogLevel::INFO,
       dateTime.toString(),
       std::string(oss.str()+"-gtfs"),
       std::to_string(cote::util::calcAltitudeKm(satEciPosn))
      );
      satId2Sensor[SAT_ID]->triggerSense();
      satId2ThresholdKm[SAT_ID] =
       threshCoeff*cote::util::calcAltitudeKm(satEciPosn);
    }
    // stepCount is assumed to be 100us; log position every 10s for sanity check
    if(stepCount%100000==0) {
      const std::array<double,3> satEciPosn = satellite.getECIPosn();
      std::ostringstream oss;
      oss << "sat-" << std::setw(5) << std::setfill('0') << SAT_ID;
      log.meas(
       cote::LogLevel::INFO,
       dateTime.toString(),
       std::string(oss.str()+"-x"),
       std::to_string(satEciPosn.at(0))
      );
      log.meas(
       cote::LogLevel::INFO,
       dateTime.toString(),
       std::string(oss.str()+"-y"),
       std::to_string(satEciPosn.at(1))
      );
      log.meas(
       cote::LogLevel::INFO,
       dateTime.toString(),
       std::string(oss.str()+"-z"),
       std::to_string(satEciPosn.at(2))
      );
    }
    // Update simulation to the next time step
    dateTime.update(hourStep,minuteStep,secondStep,nanosecondStep);
    satellite.update(hourStep,minuteStep,secondStep,nanosecondStep);
    //// Sensor position must be set before calling update, which handles the
    //// sensor trigger and records the position of the trigger
    satId2Sensor[SAT_ID]->setECIPosn(satellite.getECIPosn());
    satId2Sensor[SAT_ID]->update(hourStep,minuteStep,secondStep,nanosecondStep);
    stepCount+=1;
  }
  // Write out logs
  log.writeAll();
  // Clean up
  for(
   std::map<uint32_t,cote::Sensor*>::iterator it=satId2Sensor.begin();
   it!=satId2Sensor.end(); it++
  ) {
    delete it->second;
  }
  std::exit(EXIT_SUCCESS);
}
