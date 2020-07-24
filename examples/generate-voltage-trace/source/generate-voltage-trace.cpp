// generate-voltage-trace.cpp
//
// Generates a voltage trace for an energy-harvesting system attached to a
// satellite.
//
// Requires C++17 (e.g. GCC 8) for std::filesystem
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cmath>            // round
#include <cstdlib>          // exit, EXIT_SUCCESS
#include <filesystem>       // path
#include <fstream>          // ifstream
#include <iostream>         // cout
#include <ostream>          // endl
#include <string>           // string
#include <vector>           // vector

// cote
#include <Capacitor.hpp>    // Capacitor
#include <constants.hpp>    // constants
#include <DateTime.hpp>     // DateTime
#include <Log.hpp>          // Log
#include <LogLevel.hpp>     // LogLevel
#include <Satellite.hpp>    // Satellite
#include <SolarArray.hpp>   // SolarArray
#include <StateMachine.hpp> // StateMachine
#include <utilities.hpp>    // calcJulianDayFromYMD, calcSecSinceMidnight

int main(int argc, char** argv) {
  // Set up variables
  std::filesystem::path dateTimeFile;   // date and time file
  std::filesystem::path timeStepFile;   // file specifies time between steps
  std::filesystem::path numStepsFile;   // file specifies time step count
  std::filesystem::path satelliteFile;  // satellite file
  std::filesystem::path solarArrayFile; // solar array file
  std::filesystem::path capacitorFile;  // capacitor file
  std::filesystem::path computerSMFile; // computer state machine file
  std::filesystem::path cameraSMFile;   // camera state machine file
  std::filesystem::path adacsSMFile;    // ADACS state machine file
  std::filesystem::path logDirectory;   // logs destination
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
      } else if(pathStr.substr(pathStr.size()-15,15)=="solar-array.dat") {
        solarArrayFile = it->path();
      } else if(pathStr.substr(pathStr.size()-13,13)=="capacitor.dat") {
        capacitorFile = it->path();
      } else if(
         pathStr.substr(pathStr.size()-26,26)=="computer-state-machine.dat"
        ) {
        computerSMFile = it->path();
      } else if(
         pathStr.substr(pathStr.size()-23,23)=="adacs-state-machine.dat"
        ) {
        adacsSMFile = it->path();
      } else if(
         pathStr.substr(pathStr.size()-24,24)=="camera-state-machine.dat"
        ) {
        cameraSMFile = it->path();
      }
      it++;
    }
    // Set log directory
    logDirectory = std::filesystem::path(argv[2]);
  }
  // Set up log
  std::vector<cote::LogLevel> levels = {cote::LogLevel::INFO};
  cote::Log log(levels,logDirectory.string());
  // Set up date and time
  std::ifstream dateTimeHandle(dateTimeFile.string());
  std::string line = "";
  std::getline(dateTimeHandle,line); // Read header
  std::getline(dateTimeHandle,line); // Read values
  dateTimeHandle.close();
  int16_t year = static_cast<int16_t>(std::stoll(line.substr(0,4)));
  uint8_t month = static_cast<uint8_t>(std::stoll(line.substr(5,2)));
  uint8_t day = static_cast<uint8_t>(std::stoll(line.substr(8,2)));
  uint8_t hour = static_cast<uint8_t>(std::stoll(line.substr(11,2)));
  uint8_t minute = static_cast<uint8_t>(std::stoll(line.substr(14,2)));
  uint8_t second = static_cast<uint8_t>(std::stoll(line.substr(17,2)));
  uint32_t nanosecond = static_cast<uint32_t>(std::stoll(line.substr(20,9)));
  cote::DateTime dateTime(year,month,day,hour,minute,second,nanosecond);
  // Set up time step variables
  std::ifstream timeStepHandle(timeStepFile.string());
  line = "";
  std::getline(timeStepHandle,line); // Read header
  std::getline(timeStepHandle,line); // Read values
  timeStepHandle.close();
  uint8_t hourStep = static_cast<uint8_t>(std::stoll(line.substr(0,2)));
  uint8_t minuteStep = static_cast<uint8_t>(std::stoll(line.substr(3,2)));
  uint8_t secondStep = static_cast<uint8_t>(std::stoll(line.substr(6,2)));
  uint32_t nanosecondStep = static_cast<uint32_t>(std::stoll(line.substr(9,9)));
  double timeStepSec =
   (
    static_cast<double>(hourStep)*static_cast<double>(cote::cnst::MIN_PER_HOUR)+
    static_cast<double>(minuteStep)
   )*static_cast<double>(cote::cnst::SEC_PER_MIN)+
   static_cast<double>(secondStep)+
   static_cast<double>(nanosecondStep)/
   static_cast<double>(cote::cnst::NS_PER_SEC);
  // Set up step count
  std::ifstream numStepsHandle(numStepsFile.string());
  line = "";
  std::getline(numStepsHandle,line); // Read header
  std::getline(numStepsHandle,line); // Read values
  numStepsHandle.close();
  uint64_t numSteps = static_cast<uint64_t>(std::stoll(line.substr(0,19)));
  // Set up satellite
  cote::Satellite satellite(satelliteFile.string(),&dateTime,&log);
  // Set up solar array
  std::ifstream solarArrayHandle(solarArrayFile.string());
  line = "";
  std::getline(solarArrayHandle,line); // Read header
  std::getline(solarArrayHandle,line); // Read values
  solarArrayHandle.close();
  double openCircuitVoltage = static_cast<double>(std::stod(line.substr(0,10)));
  double surfaceAreaM2 = static_cast<double>(std::stod(line.substr(11,16)));
  double efficiency = static_cast<double>(std::stod(line.substr(28,8)));
  cote::SolarArray solarArray(
   openCircuitVoltage,surfaceAreaM2,efficiency,0,&log
  );
  // Set up capacitor
  std::ifstream capacitorHandle(capacitorFile.string());
  line = "";
  std::getline(capacitorHandle,line); // Read header
  std::getline(capacitorHandle,line); // Read values
  capacitorHandle.close();
  double capacitanceFarad = static_cast<double>(std::stod(line.substr(0,19)));
  double esrOhm = static_cast<double>(std::stod(line.substr(20,10)));
  double chargeCoulomb = static_cast<double>(std::stod(line.substr(31,32)));
  cote::Capacitor capacitor(capacitanceFarad,esrOhm,0,&log);
  capacitor.setChargeCoulomb(chargeCoulomb);
  // Set up computer state machine
  cote::StateMachine computerStateMachine(computerSMFile.string(),1,&log);
  // Set up camera state machine
  cote::StateMachine cameraStateMachine(cameraSMFile.string(),2,&log);
  // Set up ADACS state machine
  cote::StateMachine adacsStateMachine(adacsSMFile.string(),3,&log);
  // Simulation loop
  uint64_t gtfTrigger = 0;
  uint64_t stepCount = 0;
  while(stepCount<numSteps) {
    // Simulation logic
    const double JD = cote::util::calcJulianDayFromYMD(
     dateTime.getYear(),dateTime.getMonth(),dateTime.getDay()
    );
    const uint32_t SEC = cote::util::calcSecSinceMidnight(
     dateTime.getHour(), dateTime.getMinute(), dateTime.getSecond()
    );
    const uint32_t NS = dateTime.getNanosecond();
    std::array<double,3> sunEciPosnKm = cote::util::calcSunEciPosnKm(JD,SEC,NS);
    std::array<double,3> satEciPosnKm = satellite.getECIPosn();
    const double sunOcclusionFactor =
     cote::util::calcSunOcclusionFactor(satEciPosnKm,sunEciPosnKm);
    //// When sunOcclusionFactor is 0.0, irradiance is SOLAR_CONSTANT
    //// When sunOcclusionFactor is 1.0, irradiance is 1% of SOLAR_CONSTANT
    const double irradianceWPerM2 =
     -1352.44*sunOcclusionFactor+cote::cnst::SOLAR_CONSTANT;
    solarArray.setIrradianceWpM2(irradianceWPerM2); // sets solar array amps
    double powerW =
     computerStateMachine.getVariableValue("power-w")+
     cameraStateMachine.getVariableValue("power-w")+
     adacsStateMachine.getVariableValue("power-w");
    double nodeVoltageDiscriminant = cote::util::calcNodeVoltageDiscriminant(
     capacitor.getChargeCoulomb(), capacitor.getCapacitanceFarad(),
     solarArray.getCurrentAmpere(), capacitor.getEsrOhm(), powerW
    );
    if(nodeVoltageDiscriminant<0.0) { // if power draw too high, set all to off
      computerStateMachine.setCurrentState("OFF");
      cameraStateMachine.setCurrentState("OFF");
      adacsStateMachine.setCurrentState("OFF");
      powerW =
       computerStateMachine.getVariableValue("power-w")+
       cameraStateMachine.getVariableValue("power-w")+
       adacsStateMachine.getVariableValue("power-w");
      nodeVoltageDiscriminant = cote::util::calcNodeVoltageDiscriminant(
       capacitor.getChargeCoulomb(), capacitor.getCapacitanceFarad(),
       solarArray.getCurrentAmpere(), capacitor.getEsrOhm(), powerW
      );
    }
    const double nodeVoltage = cote::util::calcNodeVoltage(
     nodeVoltageDiscriminant, capacitor.getChargeCoulomb(),
     capacitor.getCapacitanceFarad(), solarArray.getCurrentAmpere(),
     capacitor.getEsrOhm()
    );
    if(
     solarArray.getOpenCircuitVoltage()<=nodeVoltage &&
     solarArray.getCurrentAmpere()!=0.0
    ) { // Zero out solar array current if node voltage is too high
      solarArray.setCurrentAmpere(0.0);
    }
    // Computer state machine custom logic
    computerStateMachine.setVariableValue("node-voltage",nodeVoltage);
    if(computerStateMachine.getCurrentState()=="WORK") {
      const double taskDurationS =
       computerStateMachine.getConstantValue("task-duration-s");
      double workTimeS = computerStateMachine.getVariableValue("work-time-s");
      std::size_t claimedTaskCount = std::round(
       computerStateMachine.getVariableValue("claimed-task-count")
      );
      while(workTimeS>=taskDurationS && claimedTaskCount>0) {
        claimedTaskCount -= 1;
        workTimeS -= taskDurationS;
      }
      computerStateMachine.setVariableValue("work-time-s",workTimeS);
      computerStateMachine.setVariableValue(
       "claimed-task-count",static_cast<double>(claimedTaskCount)
      );
    }
    // Camera state machine custom logic
    cameraStateMachine.setVariableValue("node-voltage",nodeVoltage);
    if(cameraStateMachine.getCurrentState()=="IMAGING") {
      const double imagingDurationS =
       cameraStateMachine.getConstantValue("imaging-duration-s");
      double imagingTimeS =
       cameraStateMachine.getVariableValue("imaging-time-s");
      std::size_t imagingTaskCount = std::round(
       cameraStateMachine.getVariableValue("imaging-task-count")
      );
      std::size_t readoutTaskCount = std::round(
       cameraStateMachine.getVariableValue("readout-task-count")
      );
      while(imagingTimeS>=imagingDurationS && imagingTaskCount>0) {
        readoutTaskCount += 1;
        imagingTaskCount -= 1;
        imagingTimeS -= imagingDurationS;
      }
      cameraStateMachine.setVariableValue("imaging-time-s",imagingTimeS);
      cameraStateMachine.setVariableValue(
       "imaging-task-count",static_cast<double>(imagingTaskCount)
      );
      cameraStateMachine.setVariableValue(
       "readout-task-count",static_cast<double>(readoutTaskCount)
      );
    } else if(cameraStateMachine.getCurrentState()=="READOUT") {
      const double readoutDurationS =
       cameraStateMachine.getConstantValue("readout-duration-s");
      double readoutTimeS =
       cameraStateMachine.getVariableValue("readout-time-s");
      std::size_t readoutTaskCount = std::round(
       cameraStateMachine.getVariableValue("readout-task-count")
      );
      std::size_t claimedTaskCount = std::round(
       computerStateMachine.getVariableValue("claimed-task-count")
      );
      while(readoutTimeS>=readoutDurationS && readoutTaskCount>0) {
        claimedTaskCount += 32; // 32 computer tasks per 1 image readout
        readoutTaskCount -= 1;
        readoutTimeS -= readoutDurationS;
      }
      cameraStateMachine.setVariableValue("readout-time-s",readoutTimeS);
      cameraStateMachine.setVariableValue(
       "readout-task-count",static_cast<double>(readoutTaskCount)
      );
      computerStateMachine.setVariableValue(
       "claimed-task-count",static_cast<double>(claimedTaskCount)
      );
    }
    // ADACS state machine custom logic
    adacsStateMachine.setVariableValue("node-voltage",nodeVoltage);
    // Trigger a new ground track frame every 1.7 seconds
    if(gtfTrigger==0) {
      std::size_t imagingTaskCount = std::round(
       cameraStateMachine.getVariableValue("imaging-task-count")
      );
      cameraStateMachine.setVariableValue(
       "imaging-task-count",static_cast<double>(imagingTaskCount+1)
      );
    }
    // Log the results once per millisecond
    if((stepCount%50)==0) {
      log.meas(
       cote::LogLevel::INFO,
       dateTime.toString(),
       std::string("node-voltage"),
       std::to_string(nodeVoltage)
      );
      log.meas(
       cote::LogLevel::INFO,
       dateTime.toString(),
       std::string("computer-power-state"),
       computerStateMachine.getCurrentState()
      );
      log.meas(
       cote::LogLevel::INFO,
       dateTime.toString(),
       std::string("camera-power-state"),
       cameraStateMachine.getCurrentState()
      );
    }
    // Update simulation to the next time step
    dateTime.update(hourStep,minuteStep,secondStep,nanosecondStep);
    satellite.update(hourStep,minuteStep,secondStep,nanosecondStep);
    double capacitorChargeCoulomb = capacitor.getChargeCoulomb();
    const double totalLoadCurrentAmpere = (
     computerStateMachine.getVariableValue("power-w")+
     cameraStateMachine.getVariableValue("power-w")+
     adacsStateMachine.getVariableValue("power-w")
    )/nodeVoltage;
    capacitorChargeCoulomb += (
     solarArray.getCurrentAmpere()-totalLoadCurrentAmpere
    )*timeStepSec;
    if(capacitorChargeCoulomb<0.0) {
      capacitorChargeCoulomb = 0.0;
    }
    capacitor.setChargeCoulomb(capacitorChargeCoulomb);
    if(computerStateMachine.getCurrentState()=="WORK") {
      double workTimeS = computerStateMachine.getVariableValue("work-time-s");
      computerStateMachine.setVariableValue(
       "work-time-s",workTimeS+timeStepSec
      );
    }
    if(cameraStateMachine.getCurrentState()=="IMAGING") {
      double imagingTimeS =
       cameraStateMachine.getVariableValue("imaging-time-s");
      cameraStateMachine.setVariableValue(
       "imaging-time-s",imagingTimeS+timeStepSec
      );
    } else if(cameraStateMachine.getCurrentState()=="READOUT") {
      double readoutTimeS =
       cameraStateMachine.getVariableValue("readout-time-s");
      cameraStateMachine.setVariableValue(
       "readout-time-s",readoutTimeS+timeStepSec
      );
    }
    computerStateMachine.updateState();
    cameraStateMachine.updateState();
    adacsStateMachine.updateState();
    gtfTrigger = (gtfTrigger+1)%85000;
    stepCount+=1;
  }
  // Write out logs
  log.writeAll();
  // Clean up
  std::exit(EXIT_SUCCESS);
}
