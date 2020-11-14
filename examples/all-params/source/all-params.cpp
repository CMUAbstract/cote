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
#include <string>            // string
#include <sstream>           // ostringstream
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
  std::vector<std::filesystem::path> rxSatFiles;         // sat RX configs
  std::vector<std::filesystem::path> txSatFiles;         // sat TX configs
  std::vector<std::filesystem::path> adacsSMFiles;       // ADACS power states
  std::vector<std::filesystem::path> cameraSMFiles;      // camera power states
  std::vector<std::filesystem::path> rxSatSMFiles;       // sat RX power states
  std::vector<std::filesystem::path> txSatSMFiles;       // sat TX power states
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
  // Set up satellite RX
  std::map<uint32_t,cote::Receiver*> satId2Rx;
  for(std::size_t i=0; i<rxSatFiles.size(); i++) {
    std::ifstream rxSatHandle(rxSatFiles.at(i).string());
    line = "";
    std::getline(rxSatHandle,line); // Read header
    std::getline(rxSatHandle,line); // Read values
    rxSatHandle.close();
    const uint32_t id = static_cast<uint32_t>(std::stoul(line.substr(0,10)));
    std::array<double,3> posn = {0.0,0.0,0.0};
    for(std::size_t j=0; j<satellites.size(); j++) {
      if(satellites.at(j).getID()==id) {
        posn = satellites.at(j).getECIPosn();
        j = satellites.size();
      }
    }
    const double gainDB = std::stod(line.substr(11,13));
    const double rxCenterFrequencyHz = std::stod(line.substr(25,22));
    const double rxBandwidthHz = std::stod(line.substr(48,23));
    satId2Rx[id] = new cote::Receiver(posn,gainDB,&dateTime,id,&log);
  }
  // Set up satellite TX
  std::map<uint32_t,cote::Transmitter*> satId2Tx;
  for(std::size_t i=0; i<txSatFiles.size(); i++) {
    std::ifstream txSatHandle(txSatFiles.at(i).string());
    line = "";
    std::getline(txSatHandle,line); // Read header
    std::getline(txSatHandle,line); // Read values
    txSatHandle.close();
    const uint32_t id = static_cast<uint32_t>(std::stoul(line.substr(0,10)));
    std::array<double,3> posn = {0.0,0.0,0.0};
    for(std::size_t j=0; j<satellites.size(); j++) {
      if(satellites.at(j).getID()==id) {
        posn = satellites.at(j).getECIPosn();
        j = satellites.size();
      }
    }
    const double powerW = std::stod(line.substr(11,16));
    const double lineLossDB = std::stod(line.substr(28,13));
    const double gainDB = std::stod(line.substr(42,13));
    const double txCenterFrequencyHz = std::stod(line.substr(56,22));
    const double txBandwidthHz = std::stod(line.substr(79,23));
    satId2Tx[id] = new cote::Transmitter(
     posn,powerW,lineLossDB,gainDB,&dateTime,id,&log
    );
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
  // Set up sensors
  std::map<uint32_t,cote::Sensor*> satId2Sensor;
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
    std::array<double,3> posn = {0.0,0.0,0.0};
    for(std::size_t j=0; j<satellites.size(); j++) {
      if(satellites.at(j).getID()==id) {
        posn = satellites.at(j).getECIPosn();
        j = satellites.size();
      }
    }
    const double threshCoeff = // frame width (km) = threshCoeff*altitude(km)
     (static_cast<double>(pixelCountW)*pixelSizeM/focalLengthM);
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
  // Set up ground station RX
  std::map<uint32_t,cote::Receiver*> gndId2Rx;
  for(std::size_t i=0; i<rxGndFiles.size(); i++) {
    std::ifstream rxGndHandle(rxGndFiles.at(i).string());
    line = "";
    std::getline(rxGndHandle,line); // Read header
    std::getline(rxGndHandle,line); // Read values
    rxGndHandle.close();
    const uint32_t id = static_cast<uint32_t>(std::stoul(line.substr(0,10)));
    std::array<double,3> posn = {0.0,0.0,0.0};
    for(std::size_t j=0; j<groundStations.size(); j++) {
      if(groundStations.at(j).getID()==id) {
        posn = groundStations.at(j).getECIPosn();
        j = groundStations.size();
      }
    }
    const double gainDB = std::stod(line.substr(11,13));
    const double rxCenterFrequencyHz = std::stod(line.substr(25,22));
    const double rxBandwidthHz = std::stod(line.substr(48,23));
    gndId2Rx[id] = new cote::Receiver(posn,gainDB,&dateTime,id,&log);
  }
  // Set up ground station TX
  std::map<uint32_t,cote::Transmitter*> gndId2Tx;
  for(std::size_t i=0; i<txGndFiles.size(); i++) {
    std::ifstream txGndHandle(txGndFiles.at(i).string());
    line = "";
    std::getline(txGndHandle,line); // Read header
    std::getline(txGndHandle,line); // Read values
    txGndHandle.close();
    const uint32_t id = static_cast<uint32_t>(std::stoul(line.substr(0,10)));
    std::array<double,3> posn = {0.0,0.0,0.0};
    for(std::size_t j=0; j<groundStations.size(); j++) {
      if(groundStations.at(j).getID()==id) {
        posn = groundStations.at(j).getECIPosn();
        j = groundStations.size();
      }
    }
    const double powerW = std::stod(line.substr(11,16));
    const double lineLossDB = std::stod(line.substr(28,13));
    const double gainDB = std::stod(line.substr(42,13));
    const double txCenterFrequencyHz = std::stod(line.substr(56,22));
    const double txBandwidthHz = std::stod(line.substr(79,23));
    gndId2Tx[id] = new cote::Transmitter(
     posn,powerW,lineLossDB,gainDB,&dateTime,id,&log
    );
  }

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
