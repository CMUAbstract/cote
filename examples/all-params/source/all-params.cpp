// all-params.cpp
//
// Simulates bent-pipe constellations of satellites and ground stations with
// additional configuration parameters.
//
// Requires C++17 (e.g. GCC 8) for std::filesystem
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <algorithm>         // max, sort
#include <cmath>             // round
#include <cstdlib>           // exit, EXIT_SUCCESS
#include <filesystem>        // path
#include <fstream>           // ifstream
#include <iomanip>           // setw, setfill
#include <iostream>          // cout
#include <map>               // map
#include <ostream>           // endl
#include <queue>             // queue
#include <string>            // string
#include <sstream>           // ostringstream
#include <utility>           // make_pair
#include <vector>            // vector

// json
#include <Array.hpp>         // Array
#include <Bool.hpp>          // Bool
#include <Nul.hpp>           // Nul
#include <Number.hpp>        // Number
#include <Object.hpp>        // Object
#include <String.hpp>        // String
#include <json-util.hpp>     // readFile, writeFile
#include <Value.hpp>         // Value

// cote
#include <Capacitor.hpp>     // Capacitor
#include <Channel.hpp>       // Channel
#include <constants.hpp>     // constants
#include <DateTime.hpp>      // DateTime
#include <GroundStation.hpp> // GroundStation
#include <Log.hpp>           // Log
#include <LogLevel.hpp>      // LogLevel
#include <Receiver.hpp>      // Receiver
#include <Satellite.hpp>     // Satellite
#include <Sensor.hpp>        // Sensor
#include <SolarArray.hpp>    // SolarArray
#include <StateMachine.hpp>  // StateMachine
#include <Transmitter.hpp>   // Transmitter
#include <utilities.hpp>     // calcJulianDayFromYMD, calcSecSinceMidnight

/*
// Neural network struct
namespace cote {
  typedef struct neural_network {
    const uint32_t inputWidthPx;
    const uint32_t idealFeatureWidthPx;
    const double   actualFeatureWidthM;
    const uint32_t inputHeightPx;
    const uint32_t idealFeatureHeightPx;
    const double   actualFeatureHeightM;
    const uint32_t modelBits;
    const uint32_t id;
  } NeuralNetwork;
}
*/

// Neural network struct
namespace cote {
  typedef struct neural_network {
    uint32_t inputWidthPx;
    uint32_t idealFeatureWidthPx;
    double   actualFeatureWidthM;
    uint32_t inputHeightPx;
    uint32_t idealFeatureHeightPx;
    double   actualFeatureHeightM;
    uint32_t modelBits;
    uint32_t id;
  } NeuralNetwork;
}

int main(int argc, char** argv) {
  // Set up configuration variables
  std::filesystem::path dateTimeFile;                    // start date and time
  std::filesystem::path timeStepFile;                    // time between steps
  std::filesystem::path numStepsFile;                    // time step count
  std::vector<std::filesystem::path> satelliteFiles;     // satellite TLEs
  std::vector<std::filesystem::path> solarArrayFiles;    // solar array configs
  std::vector<std::filesystem::path> capacitorFiles;     // capacitor configs
  std::vector<std::filesystem::path> adacsSMFiles;       // ADACS power states
  std::vector<std::filesystem::path> cameraSMFiles;      // camera power states
  std::vector<std::filesystem::path> computerSMFiles;    // computer power state
  std::vector<std::filesystem::path> rxSatSMFiles;       // sat RX power states
  std::vector<std::filesystem::path> txSatSMFiles;       // sat TX power states
  std::vector<std::filesystem::path> rxSatFiles;         // sat RX configs
  std::vector<std::filesystem::path> txSatFiles;         // sat TX configs
  std::vector<std::filesystem::path> sensorSatFiles;     // bits per sense etc
  std::vector<std::filesystem::path> programSatFiles;    // user applications
  std::vector<std::filesystem::path> constellationFiles; // constelation configs
  std::vector<std::filesystem::path> groundStationFiles; // gnd locations
  std::vector<std::filesystem::path> rxGndFiles;         // gnd RX configs
  std::vector<std::filesystem::path> txGndFiles;         // gnd TX configs
  std::filesystem::path logDirectory;                    // logs destination
  // Parse command line arguments
  if(argc!=3) {
    std::cout << "Usage: " << argv[0]
              << " /path/to/configuration/"
              << " /path/to/logs/"
              << std::endl;
    std::exit(EXIT_SUCCESS);
  } else {
    // Populate configuration variables
    std::filesystem::path configurationDir(argv[1]);
    std::filesystem::directory_iterator it(configurationDir);
    while(it!=std::filesystem::end(it)) {
      std::string pathStr = it->path().string();
      if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-13),13)==
       "date-time.dat"
      ) {
        dateTimeFile = it->path();
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-13),13)==
       "time-step.dat"
      ) {
        timeStepFile = it->path();
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-13),13)==
       "num-steps.dat"
      ) {
        numStepsFile = it->path();
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-4),4)==
       ".sat"
      ) {
        satelliteFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-30),16)==
       "solar-array-sat-"
      ) {
        solarArrayFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-28),14)==
       "capacitor-sat-"
      ) {
        capacitorFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-27),13)==
       "adacs-sm-sat-"
      ) {
        adacsSMFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-28),14)==
       "camera-sm-sat-"
      ) {
        cameraSMFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-30),16)==
       "computer-sm-sat-"
      ) {
        computerSMFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-24),10)==
       "rx-sm-sat-"
      ) {
        rxSatSMFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-24),10)==
       "tx-sm-sat-"
      ) {
        txSatSMFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-21),7)==
       "rx-sat-"
      ) {
        rxSatFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-21),7)==
       "tx-sat-"
      ) {
        txSatFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-25),11)==
       "sensor-sat-"
      ) {
        sensorSatFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-26),12)==
       "program-sat-"
      ) {
        programSatFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-29),14)==
       "constellation-"
      ) {
        constellationFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-4),4)==
       ".gnd"
      ) {
        groundStationFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-21),7)==
       "rx-gnd-"
      ) {
        rxGndFiles.push_back(it->path());
      } else if(
       pathStr.substr(std::max(0,static_cast<int>(pathStr.size())-21),7)==
       "tx-gnd-"
      ) {
        txGndFiles.push_back(it->path());
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
  const int16_t year = static_cast<int16_t>(std::stoi(line.substr(0,4)));
  const uint8_t month = static_cast<uint8_t>(std::stoi(line.substr(5,2)));
  const uint8_t day = static_cast<uint8_t>(std::stoi(line.substr(8,2)));
  const uint8_t hour = static_cast<uint8_t>(std::stoi(line.substr(11,2)));
  const uint8_t minute = static_cast<uint8_t>(std::stoi(line.substr(14,2)));
  const uint8_t second = static_cast<uint8_t>(std::stoi(line.substr(17,2)));
  const uint32_t ns = static_cast<uint32_t>(std::stoul(line.substr(20,9)));
  cote::DateTime dateTime(year,month,day,hour,minute,second,ns);
  // Set up time step variables
  std::ifstream timeStepHandle(timeStepFile.string());
  line = "";
  std::getline(timeStepHandle,line); // Read header
  std::getline(timeStepHandle,line); // Read values
  timeStepHandle.close();
  const uint8_t hourStep = static_cast<uint8_t>(std::stoi(line.substr(0,2)));
  const uint8_t minuteStep = static_cast<uint8_t>(std::stoi(line.substr(3,2)));
  const uint8_t secondStep = static_cast<uint8_t>(std::stoi(line.substr(6,2)));
  const uint32_t nsStep = static_cast<uint32_t>(std::stoul(line.substr(9,9)));
  const double totalStepInSec =
   (
    static_cast<double>(hourStep)*static_cast<double>(cote::cnst::MIN_PER_HOUR)+
    static_cast<double>(minuteStep)
   )*static_cast<double>(cote::cnst::SEC_PER_MIN)+
   static_cast<double>(secondStep)+
   static_cast<double>(nsStep)/static_cast<double>(cote::cnst::NS_PER_SEC);
  // Set up step count
  std::ifstream numStepsHandle(numStepsFile.string());
  line = "";
  std::getline(numStepsHandle,line); // Read header
  std::getline(numStepsHandle,line); // Read values
  numStepsHandle.close();
  const uint64_t numSteps =
   static_cast<uint64_t>(std::stoull(line.substr(0,19)));
  // Set up satellites
  std::vector<cote::Satellite> satellites;
  for(std::size_t i=0; i<satelliteFiles.size(); i++) {
    std::string satelliteFileStr = satelliteFiles.at(i).string();
    std::ifstream satelliteHandle(satelliteFileStr);
    line = "";
    std::getline(satelliteHandle,line); // Read header
    std::getline(satelliteHandle,line); // Read values
    satelliteHandle.close();
    const int16_t satYear = static_cast<int16_t>(std::stoi(line.substr(0,4)));
    const uint8_t satMonth = static_cast<uint8_t>(std::stoi(line.substr(5,2)));
    const uint8_t satDay = static_cast<uint8_t>(std::stoi(line.substr(8,2)));
    const uint8_t satHr = static_cast<uint8_t>(std::stoi(line.substr(11,2)));
    const uint8_t satMin = static_cast<uint8_t>(std::stoi(line.substr(14,2)));
    const uint8_t satSec = static_cast<uint8_t>(std::stoi(line.substr(17,2)));
    const uint32_t satNs = static_cast<uint32_t>(std::stoul(line.substr(20,9)));
    const uint32_t id = static_cast<uint32_t>(std::stoul(line.substr(30,10)));
    const cote::DateTime satTime(
     satYear,satMonth,satDay,satHr,satMin,satSec,satNs
    );
    satellites.push_back(cote::Satellite(satelliteFileStr,&dateTime,&log));
    satellites.back().setID(id);
    satellites.back().setLocalTime(satTime);
  }
  std::sort(
   satellites.begin(), satellites.end(),
   [](const cote::Satellite& s1, const cote::Satellite& s2) {
    return s1.getID() < s2.getID();
   }
  );
  std::map<uint32_t,cote::Satellite*> satId2Sat;
  for(std::size_t i=0; i<satellites.size(); i++) {
    satId2Sat[satellites.at(i).getID()] = &(satellites.at(i));
  }
  // Set up solar arrays
  std::map<uint32_t,cote::SolarArray*> satId2SolarArray;
  for(std::size_t i=0; i<solarArrayFiles.size(); i++) {
    std::ifstream solarArrayHandle(solarArrayFiles.at(i).string());
    line = "";
    std::getline(solarArrayHandle,line); // Read header
    std::getline(solarArrayHandle,line); // Read values
    solarArrayHandle.close();
    const double openCircuitVoltage = std::stod(line.substr(0,10));
    const double surfaceAreaM2 = std::stod(line.substr(11,16));
    const double efficiency = std::stod(line.substr(28,8));
    const uint32_t id = static_cast<uint32_t>(std::stoul(line.substr(37,10)));
    satId2SolarArray[id] = new cote::SolarArray(
     openCircuitVoltage,surfaceAreaM2,efficiency,id,&log
    );
    // Set initial solar array current
    const double JD = cote::util::calcJulianDayFromYMD(
     dateTime.getYear(),dateTime.getMonth(),dateTime.getDay()
    );
    const uint32_t SEC = cote::util::calcSecSinceMidnight(
     dateTime.getHour(), dateTime.getMinute(), dateTime.getSecond()
    );
    const uint32_t NS = dateTime.getNanosecond();
    const std::array<double,3> SUN_ECI_POSN_KM =
     cote::util::calcSunEciPosnKm(JD,SEC,NS);
    const std::array<double,3> SAT_ECI_POSN_KM = satId2Sat[id]->getECIPosn();
    const double sunOcclusionFactor =
     cote::util::calcSunOcclusionFactor(SAT_ECI_POSN_KM,SUN_ECI_POSN_KM);
    //// When sunOcclusionFactor is 0.0, irradiance is SOLAR_CONSTANT
    //// When sunOcclusionFactor is 1.0, irradiance is 1% of SOLAR_CONSTANT
    //// -1352.44 is the slope needed to achieve the above effect
    const double irradianceWPerM2 =
     -1352.44*sunOcclusionFactor+cote::cnst::SOLAR_CONSTANT;
    // setIrradianceWpM2 calculates and sets the appropriate output current
    satId2SolarArray[id]->setIrradianceWpM2(irradianceWPerM2);
  }
  // Set up capacitors
  std::map<uint32_t,cote::Capacitor*> satId2Capacitor;
  for(std::size_t i=0; i<capacitorFiles.size(); i++) {
    std::ifstream capacitorHandle(capacitorFiles.at(i).string());
    line = "";
    std::getline(capacitorHandle,line); // Read header
    std::getline(capacitorHandle,line); // Read values
    capacitorHandle.close();
    const double capacitanceFarad = std::stod(line.substr(0,19));
    const double esrOhm = std::stod(line.substr(20,10));
    const double chargeCoulomb = std::stod(line.substr(31,32));
    const uint32_t id = static_cast<uint32_t>(std::stoul(line.substr(64,10)));
    satId2Capacitor[id] = new cote::Capacitor(capacitanceFarad,esrOhm,id,&log);
    satId2Capacitor[id]->setChargeCoulomb(chargeCoulomb);
  }
  // Set up ADACS state machines
  std::map<uint32_t,cote::StateMachine*> satId2AdacsSm;
  for(std::size_t i=0; i<adacsSMFiles.size(); i++) {
    std::string adacsSMFileStr = adacsSMFiles.at(i).string();
    const uint32_t id = static_cast<uint32_t>(
     std::stoi(adacsSMFileStr.substr(adacsSMFileStr.size()-14,10))
    );
    satId2AdacsSm[id] = new cote::StateMachine(adacsSMFileStr,id,&log);
  }
  // Set up camera state machines
  std::map<uint32_t,cote::StateMachine*> satId2CameraSm;
  for(std::size_t i=0; i<cameraSMFiles.size(); i++) {
    std::string cameraSMFileStr = cameraSMFiles.at(i).string();
    const uint32_t id = static_cast<uint32_t>(
     std::stoi(cameraSMFileStr.substr(cameraSMFileStr.size()-14,10))
    );
    satId2CameraSm[id] = new cote::StateMachine(cameraSMFileStr,id,&log);
  }
  // Set up computer state machines
  std::map<uint32_t,cote::StateMachine*> satId2ComputerSm;
  for(std::size_t i=0; i<computerSMFiles.size(); i++) {
    std::string computerSMFileStr = computerSMFiles.at(i).string();
    const uint32_t id = static_cast<uint32_t>(
     std::stoi(computerSMFileStr.substr(computerSMFileStr.size()-14,10))
    );
    satId2ComputerSm[id] = new cote::StateMachine(computerSMFileStr,id,&log);
  }
  // Set up satellite RX state machines
  std::map<uint32_t,cote::StateMachine*> satId2RxSm;
  for(std::size_t i=0; i<rxSatSMFiles.size(); i++) {
    std::string rxSatSMFileStr = rxSatSMFiles.at(i).string();
    const uint32_t id = static_cast<uint32_t>(
     std::stoi(rxSatSMFileStr.substr(rxSatSMFileStr.size()-14,10))
    );
    satId2RxSm[id] = new cote::StateMachine(rxSatSMFileStr,id,&log);
  }
  // Set up satellite TX state machines
  std::map<uint32_t,cote::StateMachine*> satId2TxSm;
  for(std::size_t i=0; i<txSatSMFiles.size(); i++) {
    std::string txSatSMFileStr = txSatSMFiles.at(i).string();
    const uint32_t id = static_cast<uint32_t>(
     std::stoi(txSatSMFileStr.substr(txSatSMFileStr.size()-14,10))
    );
    satId2TxSm[id] = new cote::StateMachine(txSatSMFileStr,id,&log);
  }
  // Initialize the energy system for each satellite
  std::map<uint32_t,double> satId2NodeVoltage;
  for(std::size_t i=0; i<satellites.size(); i++) {
    const uint32_t SAT_ID = satellites.at(i).getID();
    double powerW =
     satId2AdacsSm[SAT_ID]->getVariableValue("power-w")+
     satId2CameraSm[SAT_ID]->getVariableValue("power-w")+
     satId2ComputerSm[SAT_ID]->getVariableValue("power-w")+
     satId2RxSm[SAT_ID]->getVariableValue("power-w")+
     satId2TxSm[SAT_ID]->getVariableValue("power-w");
    double nodeVoltageDiscriminant = cote::util::calcNodeVoltageDiscriminant(
     satId2Capacitor[SAT_ID]->getChargeCoulomb(),
     satId2Capacitor[SAT_ID]->getCapacitanceFarad(),
     satId2SolarArray[SAT_ID]->getCurrentAmpere(),
     satId2Capacitor[SAT_ID]->getEsrOhm(), powerW
    );
    if(nodeVoltageDiscriminant<0.0) { // if power draw too high, set all to off
      satId2AdacsSm[SAT_ID]->setCurrentState("OFF");
      satId2CameraSm[SAT_ID]->setCurrentState("OFF");
      satId2ComputerSm[SAT_ID]->setCurrentState("OFF");
      satId2RxSm[SAT_ID]->setCurrentState("OFF");
      satId2TxSm[SAT_ID]->setCurrentState("OFF");
      powerW =
       satId2AdacsSm[SAT_ID]->getVariableValue("power-w")+
       satId2CameraSm[SAT_ID]->getVariableValue("power-w")+
       satId2ComputerSm[SAT_ID]->getVariableValue("power-w")+
       satId2RxSm[SAT_ID]->getVariableValue("power-w")+
       satId2TxSm[SAT_ID]->getVariableValue("power-w");
      nodeVoltageDiscriminant = cote::util::calcNodeVoltageDiscriminant(
       satId2Capacitor[SAT_ID]->getChargeCoulomb(),
       satId2Capacitor[SAT_ID]->getCapacitanceFarad(),
       satId2SolarArray[SAT_ID]->getCurrentAmpere(),
       satId2Capacitor[SAT_ID]->getEsrOhm(), powerW
      );
    }
    const double nodeVoltage = cote::util::calcNodeVoltage(
     nodeVoltageDiscriminant, satId2Capacitor[SAT_ID]->getChargeCoulomb(),
     satId2Capacitor[SAT_ID]->getCapacitanceFarad(),
     satId2SolarArray[SAT_ID]->getCurrentAmpere(),
     satId2Capacitor[SAT_ID]->getEsrOhm()
    );
    if(
     satId2SolarArray[SAT_ID]->getOpenCircuitVoltage()<=nodeVoltage &&
     satId2SolarArray[SAT_ID]->getCurrentAmpere()!=0.0
    ) { // Zero out solar array current if node voltage is too high
      satId2SolarArray[SAT_ID]->setCurrentAmpere(0.0);
    }
    // Set node voltage for all state machines
    satId2AdacsSm[SAT_ID]->setVariableValue("node-voltage",nodeVoltage);
    satId2CameraSm[SAT_ID]->setVariableValue("node-voltage",nodeVoltage);
    satId2ComputerSm[SAT_ID]->setVariableValue("node-voltage",nodeVoltage);
    satId2RxSm[SAT_ID]->setVariableValue("node-voltage",nodeVoltage);
    satId2TxSm[SAT_ID]->setVariableValue("node-voltage",nodeVoltage);
    satId2NodeVoltage[SAT_ID] = nodeVoltage;
    // Update state for each state machine
    // After calculating the initial values for all state machine variables,
    // updateState must be called so that they are in the correct state for the
    // first time step
    satId2AdacsSm[SAT_ID]->updateState();
    satId2CameraSm[SAT_ID]->updateState();
    satId2ComputerSm[SAT_ID]->updateState();
    satId2RxSm[SAT_ID]->updateState();
    satId2TxSm[SAT_ID]->updateState();
  }
  // Set up satellite RX
  std::map<uint32_t,cote::Receiver*> satId2Rx;
  std::map<uint32_t,const double> satId2RxCenterFrequencyHz;
  std::map<uint32_t,const double> satId2RxBandwidthHz;
  for(std::size_t i=0; i<rxSatFiles.size(); i++) {
    std::ifstream rxSatHandle(rxSatFiles.at(i).string());
    line = "";
    std::getline(rxSatHandle,line); // Read header
    std::getline(rxSatHandle,line); // Read values
    rxSatHandle.close();
    const uint32_t id = static_cast<uint32_t>(std::stoul(line.substr(0,10)));
    const std::array<double,3> posn = satId2Sat[id]->getECIPosn();
    const double gainDB = std::stod(line.substr(11,13));
    const double rxCenterFrequencyHz = std::stod(line.substr(25,22));
    const double rxBandwidthHz = std::stod(line.substr(48,23));
    satId2Rx[id] = new cote::Receiver(posn,gainDB,&dateTime,id,&log);
    satId2RxCenterFrequencyHz.insert(std::make_pair(id,rxCenterFrequencyHz));
    satId2RxBandwidthHz.insert(std::make_pair(id,rxBandwidthHz));
  }
  // Set up satellite TX
  std::map<uint32_t,cote::Transmitter*> satId2Tx;
  std::map<uint32_t,const double> satId2TxCenterFrequencyHz;
  std::map<uint32_t,const double> satId2TxBandwidthHz;
  for(std::size_t i=0; i<txSatFiles.size(); i++) {
    std::ifstream txSatHandle(txSatFiles.at(i).string());
    line = "";
    std::getline(txSatHandle,line); // Read header
    std::getline(txSatHandle,line); // Read values
    txSatHandle.close();
    const uint32_t id = static_cast<uint32_t>(std::stoul(line.substr(0,10)));
    const std::array<double,3> posn = satId2Sat[id]->getECIPosn();
    const double powerW = std::stod(line.substr(11,16));
    const double lineLossDB = std::stod(line.substr(28,13));
    const double gainDB = std::stod(line.substr(42,13));
    const double txCenterFrequencyHz = std::stod(line.substr(56,22));
    const double txBandwidthHz = std::stod(line.substr(79,23));
    satId2Tx[id] = new cote::Transmitter(
     posn,powerW,lineLossDB,gainDB,&dateTime,id,&log
    );
    satId2TxCenterFrequencyHz.insert(std::make_pair(id,txCenterFrequencyHz));
    satId2TxBandwidthHz.insert(std::make_pair(id,txBandwidthHz));
  }
  // Set up sensors
  std::map<uint32_t,cote::Sensor*>  satId2Sensor;
  std::map<uint32_t,const double>   satId2ThreshCoeff;
  std::map<uint32_t,const uint32_t> satId2PixelCountW;
  std::map<uint32_t,const double>   satId2PixelSizeM;
  std::map<uint32_t,const double>   satId2FocalLengthM;
  std::map<uint32_t,const uint32_t> satId2PixelCountH;
  for(std::size_t i=0; i<sensorSatFiles.size(); i++) {
    std::ifstream sensorHandle(sensorSatFiles.at(i).string());
    line = "";
    std::getline(sensorHandle,line); // Read header
    std::getline(sensorHandle,line); // Read values
    sensorHandle.close();
    const uint64_t bitsPerSense =
     static_cast<uint64_t>(std::stoull(line.substr(0,10)));
    const uint32_t pixelCountW =
     static_cast<uint32_t>(std::stoul(line.substr(11,5)));
    const double pixelSizeM =
     static_cast<double>(std::stod(line.substr(17,11)));
    const double focalLengthM =
     static_cast<double>(std::stod(line.substr(29,10)));
    const uint32_t pixelCountH =
     static_cast<uint32_t>(std::stoul(line.substr(40,5)));
    const uint32_t id = static_cast<uint32_t>(std::stoul(line.substr(46,10)));
    const std::array<double,3> posn = satId2Sat[id]->getECIPosn();
    satId2ThreshCoeff.insert(std::make_pair(
     id, // geographic frame width (km) = threshCoeff*altitude(km)
     static_cast<double>(std::max(pixelCountW,pixelCountH))*pixelSizeM/
     focalLengthM
    ));
    satId2PixelCountW.insert(std::make_pair(id,pixelCountW));
    satId2PixelSizeM.insert(std::make_pair(id,pixelSizeM));
    satId2FocalLengthM.insert(std::make_pair(id,focalLengthM));
    satId2PixelCountH.insert(std::make_pair(id,pixelCountH));
    satId2Sensor[id] = new cote::Sensor(posn,&dateTime,id,&log);
    satId2Sensor[id]->setBitsPerSense(bitsPerSense);
  }
  // Set up programs
  std::map<uint32_t,cote::NeuralNetwork> satId2Program;
  for(std::size_t i=0; i<programSatFiles.size(); i++) {
    std::ifstream programHandle(programSatFiles.at(i).string());
    line = "";
    std::getline(programHandle,line); // Read header
    std::getline(programHandle,line); // Read values
    programHandle.close();
    const uint32_t inputWidthPx =
     static_cast<uint32_t>(std::stoul(line.substr(0,5)));
    const uint32_t idealFeatureWidthPx =
     static_cast<uint32_t>(std::stoul(line.substr(6,5)));
    const double   actualFeatureWidthM =
     static_cast<double>(std::stod(line.substr(12,11)));
    const uint32_t inputHeightPx =
     static_cast<uint32_t>(std::stoul(line.substr(24,5)));
    const uint32_t idealFeatureHeightPx =
     static_cast<uint32_t>(std::stoul(line.substr(30,5)));
    const double   actualFeatureHeightM =
     static_cast<double>(std::stod(line.substr(36,11)));
    const uint32_t modelBits =
     static_cast<uint32_t>(std::stoul(line.substr(48,10)));
    const uint32_t id =
     static_cast<uint32_t>(std::stoul(line.substr(59,10)));
    cote::NeuralNetwork neuralNetwork = {
     .inputWidthPx = inputWidthPx,
     .idealFeatureWidthPx = idealFeatureWidthPx,
     .actualFeatureWidthM = actualFeatureWidthM,
     .inputHeightPx = inputHeightPx,
     .idealFeatureHeightPx = idealFeatureHeightPx,
     .actualFeatureHeightM = actualFeatureHeightM,
     .modelBits = modelBits,
     .id = id
    };
    satId2Program[id] = neuralNetwork;
  }
  // Set up constellation configuration variables
  std::map<uint32_t,std::string> constellId2DataCollection;
  std::map<uint32_t,std::string> constellId2DataProcessing;
  std::map< uint32_t, std::vector<cote::Satellite*> > constellId2Sats;
  for(std::size_t i=0; i<constellationFiles.size(); i++) {
    // Parse constellation configuration file
    std::string constellationFileStr =
     json::util::readFile(constellationFiles.at(i).string());
    json::Object* constellationConfig = new json::Object(constellationFileStr);
    std::map<std::string,json::Value*> constellConfigMap =
     constellationConfig->getValue();
    // Get constellation ID
    uint32_t constellId = static_cast<uint32_t>(std::stoul(
     dynamic_cast<json::String*>(constellConfigMap["id"])->getValue()
    ));
    // Record constellation data collection method
    std::string dataCollection = dynamic_cast<json::String*>(
     constellConfigMap["data-collection"]
    )->getValue();
    constellId2DataCollection[constellId] = dataCollection;
    // Record constellation data processing method
    std::string dataProcessing = dynamic_cast<json::String*>(
     constellConfigMap["data-processing"]
    )->getValue();
    constellId2DataProcessing[constellId] = dataProcessing;
    // Record constellation satellite members
    constellId2Sats[constellId] = std::vector<cote::Satellite*>();
    std::vector<json::Value*> constellSatMembers =
     dynamic_cast<json::Array*>(constellConfigMap["satellite-ids"])->getValue();
    std::vector<uint32_t> constellSatIds;
    for(std::size_t j=0; j<constellSatMembers.size(); j++) {
      uint32_t satId = static_cast<uint32_t>(std::stoul(
       dynamic_cast<json::String*>(constellSatMembers.at(j))->getValue()
      ));
      constellId2Sats[constellId].push_back(satId2Sat[satId]);
    }
    // Clean up
    delete constellationConfig;
    constellationConfig = nullptr;
  }
  // Set up ground stations
  std::vector<cote::GroundStation> groundStations;
  for(std::size_t i=0; i<groundStationFiles.size(); i++) {
    std::ifstream gndHandle(groundStationFiles.at(i).string());
    line = "";
    std::getline(gndHandle,line); // Read header
    std::getline(gndHandle,line); // Read values
    gndHandle.close();
    uint32_t id  = static_cast<uint32_t>(std::stoul(line.substr(0,10)));
    double   lat = std::stod(line.substr(11,13));
    double   lon = std::stod(line.substr(25,14));
    double   hae = std::stod(line.substr(40,13));
    groundStations.push_back(
     cote::GroundStation(lat,lon,hae,&dateTime,id,&log)
    );
  }
  std::sort(
   groundStations.begin(), groundStations.end(),
   [](const cote::GroundStation& g1, const cote::GroundStation& g2) {
    return g1.getID() < g2.getID();
   }
  );
  std::map<uint32_t,cote::GroundStation*> gndId2Gnd;
  for(std::size_t i=0; i<groundStations.size(); i++) {
    gndId2Gnd[groundStations.at(i).getID()] = &(groundStations.at(i));
  }
  // Set up ground station RX
  std::map<uint32_t,cote::Receiver*> gndId2Rx;
  std::map<uint32_t,const double> gndId2RxCenterFrequencyHz;
  std::map<uint32_t,const double> gndId2RxBandwidthHz;
  for(std::size_t i=0; i<rxGndFiles.size(); i++) {
    std::ifstream rxGndHandle(rxGndFiles.at(i).string());
    line = "";
    std::getline(rxGndHandle,line); // Read header
    std::getline(rxGndHandle,line); // Read values
    rxGndHandle.close();
    const uint32_t id = static_cast<uint32_t>(std::stoul(line.substr(0,10)));
    const std::array<double,3> posn = gndId2Gnd[id]->getECIPosn();
    const double gainDB = std::stod(line.substr(11,13));
    const double rxCenterFrequencyHz = std::stod(line.substr(25,22));
    const double rxBandwidthHz = std::stod(line.substr(48,23));
    gndId2Rx[id] = new cote::Receiver(posn,gainDB,&dateTime,id,&log);
    gndId2RxCenterFrequencyHz.insert(std::make_pair(id,rxCenterFrequencyHz));
    gndId2RxBandwidthHz.insert(std::make_pair(id,rxBandwidthHz));
  }
  // Set up ground station TX
  std::map<uint32_t,cote::Transmitter*> gndId2Tx;
  std::map<uint32_t,const double> gndId2TxCenterFrequencyHz;
  std::map<uint32_t,const double> gndId2TxBandwidthHz;
  for(std::size_t i=0; i<txGndFiles.size(); i++) {
    std::ifstream txGndHandle(txGndFiles.at(i).string());
    line = "";
    std::getline(txGndHandle,line); // Read header
    std::getline(txGndHandle,line); // Read values
    txGndHandle.close();
    const uint32_t id = static_cast<uint32_t>(std::stoul(line.substr(0,10)));
    const std::array<double,3> posn = gndId2Gnd[id]->getECIPosn();
    const double powerW = std::stod(line.substr(11,16));
    const double lineLossDB = std::stod(line.substr(28,13));
    const double gainDB = std::stod(line.substr(42,13));
    const double txCenterFrequencyHz = std::stod(line.substr(56,22));
    const double txBandwidthHz = std::stod(line.substr(79,23));
    gndId2Tx[id] = new cote::Transmitter(
     posn,powerW,lineLossDB,gainDB,&dateTime,id,&log
    );
    gndId2TxCenterFrequencyHz.insert(std::make_pair(id,txCenterFrequencyHz));
    gndId2TxBandwidthHz.insert(std::make_pair(id,txBandwidthHz));
  }
  // Simulation data
  std::map<uint32_t,double> satId2ThresholdKm;
  std::map<uint32_t,std::queue<double>> satId2GSDs;
  std::map<uint32_t,bool> satId2RxOccupied;
  std::map<uint32_t,bool> satId2TxOccupied;
  std::map<uint32_t,uint64_t> satId2TxBufferBits;
  for(std::size_t i=0; i<satellites.size(); i++) {
    const uint32_t SAT_ID = satellites.at(i).getID();
    satId2ThresholdKm[SAT_ID] =
     satId2ThreshCoeff[SAT_ID]*
     cote::util::calcAltitudeKm(satellites.at(i).getECIPosn());
    satId2GSDs[SAT_ID] = std::queue<double>();
    satId2RxOccupied[SAT_ID] = false;
    satId2TxOccupied[SAT_ID] = false;
    satId2TxBufferBits[SAT_ID] = 0;
  }
  std::vector<cote::Channel> crosslinks = std::vector<cote::Channel>();
  std::vector<cote::Channel> downlinks  = std::vector<cote::Channel>();
  std::vector<cote::Channel> uplinks    = std::vector<cote::Channel>();
  std::map<uint32_t,std::vector<cote::Satellite*>> gndId2VisSats;
  std::map<uint32_t,cote::Satellite*> gndId2CurrSat;
  for(std::size_t i=0; i<groundStations.size(); i++) {
    const uint32_t GND_ID = groundStations.at(i).getID();
    gndId2VisSats[GND_ID] = std::vector<cote::Satellite*>();
    gndId2CurrSat[GND_ID] = nullptr;
  }
  // Simulation loop
  uint64_t stepCount = 0;
  while(stepCount<numSteps) {
    // Date and time
    const double JD = cote::util::calcJulianDayFromYMD(
     dateTime.getYear(),dateTime.getMonth(),dateTime.getDay()
    );
    const uint32_t SEC = cote::util::calcSecSinceMidnight(
     dateTime.getHour(), dateTime.getMinute(), dateTime.getSecond()
    );
    const uint32_t NS = dateTime.getNanosecond();
    const std::array<double,3> SUN_ECI_POSN_KM =
     cote::util::calcSunEciPosnKm(JD,SEC,NS);
    // Simulate satellites
    for(std::size_t i=0; i<satellites.size(); i++) {
      const uint32_t SAT_ID = satellites.at(i).getID();
      const std::array<double,3> SAT_ECI_POSN_KM =
       satId2Sat[SAT_ID]->getECIPosn();
      // Simulate solar array current
      const double sunOcclusionFactor =
       cote::util::calcSunOcclusionFactor(SAT_ECI_POSN_KM,SUN_ECI_POSN_KM);
      //// When sunOcclusionFactor is 0.0, irradiance is SOLAR_CONSTANT
      //// When sunOcclusionFactor is 1.0, irradiance is 1% of SOLAR_CONSTANT
      //// -1352.44 is the slope needed to achieve the above effect
      const double irradianceWPerM2 =
       -1352.44*sunOcclusionFactor+cote::cnst::SOLAR_CONSTANT;
      //// setIrradianceWpM2 calculates and sets the appropriate output current
      satId2SolarArray[SAT_ID]->setIrradianceWpM2(irradianceWPerM2);
      // Simulate capacitor charge
      double capacitorChargeCoulomb =
       satId2Capacitor[SAT_ID]->getChargeCoulomb();
      const double totalLoadCurrentAmpere = (
       satId2AdacsSm[SAT_ID]->getVariableValue("power-w")+
       satId2CameraSm[SAT_ID]->getVariableValue("power-w")+
       satId2ComputerSm[SAT_ID]->getVariableValue("power-w")+
       satId2RxSm[SAT_ID]->getVariableValue("power-w")+
       satId2TxSm[SAT_ID]->getVariableValue("power-w")
      )/satId2NodeVoltage[SAT_ID];
      capacitorChargeCoulomb += (
       satId2SolarArray[SAT_ID]->getCurrentAmpere()-totalLoadCurrentAmpere
      )*totalStepInSec;
      if(capacitorChargeCoulomb<0.0) {
        capacitorChargeCoulomb = 0.0;
      }
      satId2Capacitor[SAT_ID]->setChargeCoulomb(capacitorChargeCoulomb);
      // Simulate ADACS state machine
      //// Nothing to be done
      // Simulate camera state machine
      if(satId2CameraSm[SAT_ID]->getCurrentState()=="IMAGING") {
        const double imagingDurationS =
         satId2CameraSm[SAT_ID]->getConstantValue("imaging-duration-s");
        double imagingTimeS = // this value is set in the sim update section
         satId2CameraSm[SAT_ID]->getVariableValue("imaging-time-s");
        std::size_t imagingTaskCount = std::round( // hack double to size_t
         satId2CameraSm[SAT_ID]->getVariableValue("imaging-task-count")
        );
        std::size_t readoutTaskCount = std::round( // hack double to size_t
         satId2CameraSm[SAT_ID]->getVariableValue("readout-task-count")
        );
        while(imagingTimeS>=imagingDurationS && imagingTaskCount>0) {
          readoutTaskCount += 1;
          imagingTaskCount -= 1;
          imagingTimeS -= imagingDurationS;
        }
        satId2CameraSm[SAT_ID]->setVariableValue("imaging-time-s",imagingTimeS);
        satId2CameraSm[SAT_ID]->setVariableValue(
         "imaging-task-count",static_cast<double>(imagingTaskCount)
        );
        satId2CameraSm[SAT_ID]->setVariableValue(
         "readout-task-count",static_cast<double>(readoutTaskCount)
        );
      } else if(satId2CameraSm[SAT_ID]->getCurrentState()=="READOUT") {
        const double readoutDurationS =
         satId2CameraSm[SAT_ID]->getConstantValue("readout-duration-s");
        double readoutTimeS = // this value is set in the sim update section
         satId2CameraSm[SAT_ID]->getVariableValue("readout-time-s");
        std::size_t readoutTaskCount = std::round( // hack double to size_t
         satId2CameraSm[SAT_ID]->getVariableValue("readout-task-count")
        );
        std::size_t claimedTaskCount = std::round( // hack double to size_t
         satId2ComputerSm[SAT_ID]->getVariableValue("claimed-task-count")
        );
        while(readoutTimeS>=readoutDurationS && readoutTaskCount>0) {
          // Calculate tile count per image
          const double GSD = satId2GSDs[SAT_ID].front();
          satId2GSDs[SAT_ID].pop();
          const std::size_t tileColCount = std::round(
           (static_cast<double>(satId2PixelCountW[SAT_ID])*
            static_cast<double>(satId2Program[SAT_ID].idealFeatureWidthPx)/
            (satId2Program[SAT_ID].actualFeatureWidthM/GSD)
           )/static_cast<double>(satId2Program[SAT_ID].inputWidthPx)
          );
          const std::size_t tileRowCount = std::round(
           (static_cast<double>(satId2PixelCountH[SAT_ID])*
            static_cast<double>(satId2Program[SAT_ID].idealFeatureHeightPx)/
            (satId2Program[SAT_ID].actualFeatureHeightM/GSD)
           )/static_cast<double>(satId2Program[SAT_ID].inputHeightPx)
          );
          const std::size_t tilesPerImage = tileColCount*tileRowCount;
          // State machine logic
          claimedTaskCount += tilesPerImage; // computer tasks per image readout
          readoutTaskCount -= 1;
          readoutTimeS -= readoutDurationS;
          // triggerSense() will already have been called; now, the data has
          // been read out so the satellite TX radio has data available (true)
          // for transmission to ground
          satId2TxSm[SAT_ID]->setVariableValue("data-available",1.0);
          satId2TxBufferBits[SAT_ID] += satId2Sensor[SAT_ID]->getBitsPerSense();
        }
        satId2CameraSm[SAT_ID]->setVariableValue("readout-time-s",readoutTimeS);
        satId2CameraSm[SAT_ID]->setVariableValue(
         "readout-task-count",static_cast<double>(readoutTaskCount)
        );
        satId2ComputerSm[SAT_ID]->setVariableValue(
         "claimed-task-count",static_cast<double>(claimedTaskCount)
        );
      }
      // Simulate computer state machine
      if(satId2ComputerSm[SAT_ID]->getCurrentState()=="WORK") {
        const double taskDurationS =
         satId2ComputerSm[SAT_ID]->getConstantValue("task-duration-s");
        double workTimeS = // this value is set in the sim update section
         satId2ComputerSm[SAT_ID]->getVariableValue("work-time-s");
        std::size_t claimedTaskCount = std::round( // hack double to size_t
         satId2ComputerSm[SAT_ID]->getVariableValue("claimed-task-count")
        );
        while(workTimeS>=taskDurationS && claimedTaskCount>0) {
          claimedTaskCount -= 1;
          workTimeS -= taskDurationS;
        }
        satId2ComputerSm[SAT_ID]->setVariableValue("work-time-s",workTimeS);
        satId2ComputerSm[SAT_ID]->setVariableValue(
         "claimed-task-count",static_cast<double>(claimedTaskCount)
        );
      }
      // Simulate satellite RX state machine
      //// Nothing to be done
      // Simulate satellite TX state machine
      //// Nothing to be done
      // Simulate satellite RX
      //// Nothing to be done
      // Simulate satellite TX
      //// Nothing to be done
      // Simulate satellite sensor (data collection)
      const std::array<double,3> PREV_SENSE_POSN =
       satId2Sensor[SAT_ID]->getPrevSensePosn();
      const cote::DateTime PREV_SENSE_DATE_TIME =
       satId2Sensor[SAT_ID]->getPrevSenseDateTime();
      const double PREV_SENSE_JD = cote::util::calcJulianDayFromYMD(
       PREV_SENSE_DATE_TIME.getYear(), PREV_SENSE_DATE_TIME.getMonth(),
       PREV_SENSE_DATE_TIME.getDay()
      );
      const uint32_t PREV_SENSE_SEC = cote::util::calcSecSinceMidnight(
       PREV_SENSE_DATE_TIME.getHour(), PREV_SENSE_DATE_TIME.getMinute(),
       PREV_SENSE_DATE_TIME.getSecond()
      );
      const uint32_t PREV_SENSE_NS = PREV_SENSE_DATE_TIME.getNanosecond();
      const double PREV_SENSE_LAT =
       cote::util::calcSubpointLatitude(PREV_SENSE_POSN);
      const double PREV_SENSE_LON = cote::util::calcSubpointLongitude(
       PREV_SENSE_JD, PREV_SENSE_SEC, PREV_SENSE_NS, PREV_SENSE_POSN
      );
      const double SAT_LAT =
       cote::util::calcSubpointLatitude(SAT_ECI_POSN_KM);
      const double SAT_LON = cote::util::calcSubpointLongitude(
       JD, SEC, NS, SAT_ECI_POSN_KM
      );
      const double DIST_KM = cote::util::calcGreatCircleArc(
       SAT_LON, SAT_LAT, PREV_SENSE_LON, PREV_SENSE_LAT
      )*cote::cnst::WGS_84_A; // Earth "radius" in km
      if(DIST_KM>=satId2ThresholdKm[SAT_ID]) {
        // Increment the camera state machine imaging task count
        std::size_t imagingTaskCount = std::round( // hack double to size_t
         satId2CameraSm[SAT_ID]->getVariableValue("imaging-task-count")
        );
        satId2CameraSm[SAT_ID]->setVariableValue(
         "imaging-task-count",static_cast<double>(imagingTaskCount+1)
        );
        // Update the threshold value for next GTFR sense event
        const double SAT_ALT_KM = cote::util::calcAltitudeKm(SAT_ECI_POSN_KM);
        satId2ThresholdKm[SAT_ID] = satId2ThreshCoeff[SAT_ID]*SAT_ALT_KM;
        // Trigger a sense event now to update prevSense values
        satId2Sensor[SAT_ID]->triggerSense();
        // Calculate GSD for use in tiling calculation
        satId2GSDs[SAT_ID].push(
         satId2PixelSizeM[SAT_ID]*SAT_ALT_KM*cote::cnst::M_PER_KM/
         satId2FocalLengthM[SAT_ID]
        );
      }
      // Simulate satellite program
      //// Nothing to be done
      // Simulate satellite communication channels
    }
    // Clear communication channels so that they can be re-calculated
    crosslinks.clear();
    downlinks.clear();
    uplinks.clear();
    // Determine visible satellites for each ground station
    for(std::size_t i=0; i<groundStations.size(); i++) {
      const double GND_LAT  = groundStations.at(i).getLatitude();
      const double GND_LON  = groundStations.at(i).getLongitude();
      const double GND_HAE  = groundStations.at(i).getHAE();
      const uint32_t GND_ID = groundStations.at(i).getID();
      gndId2VisSats[GND_ID].clear();
      bool currSatInView  = false;
      for(std::size_t j=0; j<satellites.size(); j++) {
        const std::array<double,3> SAT_ECI_POSN_KM =
         satellites.at(j).getECIPosn();
        if(cote::util::calcElevationDeg(
         JD, SEC, NS, GND_LAT, GND_LON, GND_HAE, SAT_ECI_POSN_KM
        )>=10.0) {
          gndId2VisSats[GND_ID].push_back(&(satellites.at(j)));
          if(
           gndId2CurrSat[GND_ID]!=nullptr &&
           gndId2CurrSat[GND_ID]==gndId2VisSats[GND_ID].back() // same pointer
          ) {
            currSatInView = true;
          }
        }
      }
      // This removes a sat/gnd channel once the satellite goes out of view
      if(!currSatInView && gndId2CurrSat[GND_ID]!=nullptr) {
        const uint32_t SAT_ID = gndId2CurrSat[GND_ID]->getID();
        satId2RxSm[SAT_ID]->setVariableValue("channel-available",0.0);
        satId2TxSm[SAT_ID]->setVariableValue("channel-available",0.0);
        satId2RxOccupied[SAT_ID] = false;
        satId2TxOccupied[SAT_ID] = false;
        gndId2CurrSat[GND_ID] = nullptr;
      }
    }
    // Separate loop to construct channels to make informed choice of satellite
    for(std::size_t i=0; i<groundStations.size(); i++) {
      const uint32_t GND_ID = groundStations.at(i).getID();
      // If no current link, choose a visible satellite
      // Best satellite policy:
      //   - visible (i.e. in gndId2VisSats)
      //   - has most data to downlink (heurisic; data may not be read out yet)
      if(gndId2CurrSat[GND_ID]==nullptr) {
        cote::Satellite* bestSat = nullptr;
        uint64_t bestSatBuffer = 0;
        for(std::size_t j=0; j<gndId2VisSats[GND_ID].size(); j++) {
          cote::Satellite* satj = gndId2VisSats[GND_ID].at(j);
          const uint32_t SAT_ID = satj->getID();
          const uint64_t BUF = satId2Sensor[SAT_ID]->getBitsBuffered();
          // Don't select a satellite that is already transmitting
          if(!satId2TxOccupied[SAT_ID] && BUF>bestSatBuffer) {
            bestSat = satj;
            bestSatBuffer = BUF;
          }
          satj = nullptr;
        }
        if(bestSat!=nullptr) {
          const uint32_t SAT_ID = bestSat->getID();
          satId2RxSm[SAT_ID]->setVariableValue("channel-available",1.0);
          satId2TxSm[SAT_ID]->setVariableValue("channel-available",1.0);
          satId2RxOccupied[SAT_ID] = true;
          satId2TxOccupied[SAT_ID] = true;
          gndId2CurrSat[GND_ID] = bestSat;
        }
        bestSat = nullptr;
      }
      // Construct downlink and uplink if available
      if(gndId2CurrSat[GND_ID]!=nullptr) {
        const uint32_t SAT_ID = gndId2CurrSat[GND_ID]->getID();
        // Construct downlink
        downlinks.push_back(
         cote::Channel(
          satId2Tx[SAT_ID],gndId2Rx[GND_ID],
          satId2TxCenterFrequencyHz[SAT_ID],satId2TxBandwidthHz[SAT_ID],
          &dateTime,&log
         )
        );
        // Data downlink
        if(satId2TxSm[SAT_ID]->getCurrentState()=="TX") {
          const uint64_t TX_BITS = static_cast<uint64_t>(std::round(
           static_cast<double>(downlinks.back().getBitsPerSec())*totalStepInSec
          ));
          satId2Sensor[SAT_ID]->drainBuffer(TX_BITS);
          if(TX_BITS<=satId2TxBufferBits[SAT_ID]) {
            satId2TxBufferBits[SAT_ID] -= TX_BITS;
          } else {
            satId2TxBufferBits[SAT_ID] = 0;
            satId2TxSm[SAT_ID]->setVariableValue("data-available",0.0);
          }
        }
        // Construct uplink
        uplinks.push_back(
         cote::Channel(
          gndId2Tx[GND_ID],satId2Rx[SAT_ID],
          gndId2TxCenterFrequencyHz[GND_ID],gndId2TxBandwidthHz[GND_ID],
          &dateTime,&log
         )
        );
      }
    }
    // Update simulation to the next time step
    dateTime.update(hourStep,minuteStep,secondStep,nsStep);
    for(std::size_t i=0; i<satellites.size(); i++) {
      const uint32_t SAT_ID = satellites.at(i).getID();
      satellites.at(i).update(hourStep,minuteStep,secondStep,nsStep);
      const std::array<double,3> SAT_ECI_POSN_KM =
       satellites.at(i).getECIPosn();
      if(satId2CameraSm[SAT_ID]->getCurrentState()=="IMAGING") {
        double imagingTimeS =
         satId2CameraSm[SAT_ID]->getVariableValue("imaging-time-s");
        satId2CameraSm[SAT_ID]->setVariableValue(
         "imaging-time-s",imagingTimeS+totalStepInSec
        );
      } else if(satId2CameraSm[SAT_ID]->getCurrentState()=="READOUT") {
        double readoutTimeS =
         satId2CameraSm[SAT_ID]->getVariableValue("readout-time-s");
        satId2CameraSm[SAT_ID]->setVariableValue(
         "readout-time-s",readoutTimeS+totalStepInSec
        );
      }
      if(satId2ComputerSm[SAT_ID]->getCurrentState()=="WORK") {
        double workTimeS =
         satId2ComputerSm[SAT_ID]->getVariableValue("work-time-s");
        satId2ComputerSm[SAT_ID]->setVariableValue(
         "work-time-s",workTimeS+totalStepInSec
        );
      }
      satId2Rx[SAT_ID]->setPosn(SAT_ECI_POSN_KM);
      satId2Rx[SAT_ID]->update(hourStep,minuteStep,secondStep,nsStep);
      satId2Tx[SAT_ID]->setPosn(SAT_ECI_POSN_KM);
      satId2Tx[SAT_ID]->update(hourStep,minuteStep,secondStep,nsStep);
      satId2Sensor[SAT_ID]->setECIPosn(SAT_ECI_POSN_KM);
      satId2Sensor[SAT_ID]->update(hourStep,minuteStep,secondStep,nsStep);
      double powerW =
       satId2AdacsSm[SAT_ID]->getVariableValue("power-w")+
       satId2CameraSm[SAT_ID]->getVariableValue("power-w")+
       satId2ComputerSm[SAT_ID]->getVariableValue("power-w")+
       satId2RxSm[SAT_ID]->getVariableValue("power-w")+
       satId2TxSm[SAT_ID]->getVariableValue("power-w");
      double nodeVoltageDiscriminant = cote::util::calcNodeVoltageDiscriminant(
       satId2Capacitor[SAT_ID]->getChargeCoulomb(),
       satId2Capacitor[SAT_ID]->getCapacitanceFarad(),
       satId2SolarArray[SAT_ID]->getCurrentAmpere(),
       satId2Capacitor[SAT_ID]->getEsrOhm(), powerW
      );
      if(nodeVoltageDiscriminant<0.0) { // if power draw too high, turn all off
        satId2AdacsSm[SAT_ID]->setCurrentState("OFF");
        satId2CameraSm[SAT_ID]->setCurrentState("OFF");
        satId2ComputerSm[SAT_ID]->setCurrentState("OFF");
        satId2RxSm[SAT_ID]->setCurrentState("OFF");
        satId2TxSm[SAT_ID]->setCurrentState("OFF");
        powerW =
         satId2AdacsSm[SAT_ID]->getVariableValue("power-w")+
         satId2CameraSm[SAT_ID]->getVariableValue("power-w")+
         satId2ComputerSm[SAT_ID]->getVariableValue("power-w")+
         satId2RxSm[SAT_ID]->getVariableValue("power-w")+
         satId2TxSm[SAT_ID]->getVariableValue("power-w");
        nodeVoltageDiscriminant = cote::util::calcNodeVoltageDiscriminant(
         satId2Capacitor[SAT_ID]->getChargeCoulomb(),
         satId2Capacitor[SAT_ID]->getCapacitanceFarad(),
         satId2SolarArray[SAT_ID]->getCurrentAmpere(),
         satId2Capacitor[SAT_ID]->getEsrOhm(), powerW
        );
      }
      const double nodeVoltage = cote::util::calcNodeVoltage(
       nodeVoltageDiscriminant, satId2Capacitor[SAT_ID]->getChargeCoulomb(),
       satId2Capacitor[SAT_ID]->getCapacitanceFarad(),
       satId2SolarArray[SAT_ID]->getCurrentAmpere(),
       satId2Capacitor[SAT_ID]->getEsrOhm()
      );
      if(
       satId2SolarArray[SAT_ID]->getOpenCircuitVoltage()<=nodeVoltage &&
       satId2SolarArray[SAT_ID]->getCurrentAmpere()!=0.0
      ) { // Zero out solar array current if node voltage is too high
        satId2SolarArray[SAT_ID]->setCurrentAmpere(0.0);
      }
      // Set node voltage for all state machines
      satId2AdacsSm[SAT_ID]->setVariableValue("node-voltage",nodeVoltage);
      satId2CameraSm[SAT_ID]->setVariableValue("node-voltage",nodeVoltage);
      satId2ComputerSm[SAT_ID]->setVariableValue("node-voltage",nodeVoltage);
      satId2RxSm[SAT_ID]->setVariableValue("node-voltage",nodeVoltage);
      satId2TxSm[SAT_ID]->setVariableValue("node-voltage",nodeVoltage);
      satId2NodeVoltage[SAT_ID] = nodeVoltage;
    }
    for(std::size_t i=0; i<groundStations.size(); i++) {
      const uint32_t GND_ID = groundStations.at(i).getID();
      groundStations.at(i).update(hourStep,minuteStep,secondStep,nsStep);
      const std::array<double,3> GND_ECI_POSN_KM =
       groundStations.at(i).getECIPosn();
      gndId2Rx[GND_ID]->setPosn(GND_ECI_POSN_KM);
      gndId2Rx[GND_ID]->update(hourStep,minuteStep,secondStep,nsStep);
      gndId2Tx[GND_ID]->setPosn(GND_ECI_POSN_KM);
      gndId2Tx[GND_ID]->update(hourStep,minuteStep,secondStep,nsStep);
    }
    // Increment step count
    stepCount+=1;
  }
  // Write out any remaining logs
  log.writeAll();
  // Clean up ground station TX
  for(
   std::map<uint32_t,cote::Transmitter*>::iterator it = gndId2Tx.begin();
   it!=gndId2Tx.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up ground station RX
  for(
   std::map<uint32_t,cote::Receiver*>::iterator it = gndId2Rx.begin();
   it!=gndId2Rx.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up ground stations
  //// Nothing to be done
  // Clean up constellation configuration variables
  for(
   std::map< uint32_t, std::vector<cote::Satellite*> >::iterator it =
   constellId2Sats.begin(); it!=constellId2Sats.end(); it++
  ) {
    for(std::size_t i=0; i<it->second.size(); i++) {
      it->second.at(i) = nullptr;
    }
  }
  // Clean up programs
  //// Nothing to be done
  // Clean up sensors
  for(
   std::map<uint32_t,cote::Sensor*>::iterator it = satId2Sensor.begin();
   it!=satId2Sensor.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up satellite TX
  for(
   std::map<uint32_t,cote::Transmitter*>::iterator it = satId2Tx.begin();
   it!=satId2Tx.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up satellite RX
  for(
   std::map<uint32_t,cote::Receiver*>::iterator it = satId2Rx.begin();
   it!=satId2Rx.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up satellite TX state machines
  for(
   std::map<uint32_t,cote::StateMachine*>::iterator it = satId2TxSm.begin();
   it!=satId2TxSm.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up satellite RX state machines
  for(
   std::map<uint32_t,cote::StateMachine*>::iterator it = satId2RxSm.begin();
   it!=satId2RxSm.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up computer state machines
  for(
   std::map<uint32_t,cote::StateMachine*>::iterator it=satId2ComputerSm.begin();
   it!=satId2ComputerSm.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up camera state machines
  for(
   std::map<uint32_t,cote::StateMachine*>::iterator it = satId2CameraSm.begin();
   it!=satId2CameraSm.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up ADACS state machines
  for(
   std::map<uint32_t,cote::StateMachine*>::iterator it = satId2AdacsSm.begin();
   it!=satId2AdacsSm.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up capacitors
  for(
   std::map<uint32_t,cote::Capacitor*>::iterator it = satId2Capacitor.begin();
   it!=satId2Capacitor.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up solar arrays
  for(
   std::map<uint32_t,cote::SolarArray*>::iterator it = satId2SolarArray.begin();
   it!=satId2SolarArray.end(); it++
  ) {
    delete it->second;
    it->second = nullptr;
  }
  // Clean up satellites
  //// Nothing to be done
  // Clean up step count
  //// Nothing to be done
  // Clean up time step variables
  //// Nothing to be done
  // Clean up date and time
  //// Nothing to be done
  std::exit(EXIT_SUCCESS);
}
