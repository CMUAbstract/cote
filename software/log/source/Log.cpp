// Log.cpp
// Log class implementation file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstddef>      // size_t
#include <cstdint>      // uint8_t
#include <fstream>      // ofstream
#include <string>       // string
#include <utility>      // pair
#include <vector>       // vector

// cote library
#include <Log.hpp>      // Log
#include <LogLevel.hpp> // LogLevel

namespace cote {
  Log::Log(const std::vector<LogLevel>& levels, const std::string& dir) {
    // ingest log levels
    this->levels = static_cast<uint8_t>(0);
    for(size_t i=0; i<levels.size(); i++) {
      this->levels |= static_cast<uint8_t>(levels.at(i));
    }
    // sanitize path to dir
    std::string sanitized_dir(dir);
    if(sanitized_dir.back()!='/') {
      sanitized_dir.push_back('/');
    }
    this->dir = sanitized_dir;
  }

  void Log::evnt(
   const LogLevel& level, const std::string& dateTime, const std::string& evnt
  ) {
    if(isEnabled(level)) {
      if(this->evntLogs.count(evnt)==0) {
        this->evntLogs[evnt] = std::vector<std::string>();
        std::ofstream outFile(this->dir+"evnt-"+evnt+".csv",std::ios::out);
        outFile << evnt << "-time,"
                << std::endl;
        outFile.close();
      }
      this->evntLogs[evnt].push_back(dateTime);
      if(this->evntLogs[evnt].size()>=BUFFER_SIZE) {
        this->writeEvnt(evnt);
        this->evntLogs[evnt].clear();
      }
    }
  }

  void Log::meas(
   const LogLevel& level, const std::string& dateTime,
   const std::string& meas, const std::string& valu
  ) {
    if(isEnabled(level)) {
      if(this->measLogs.count(meas)==0) {
        this->measLogs[meas] =
         std::vector< std::pair<std::string,std::string> >();
        std::ofstream outFile(this->dir+"meas-"+meas+".csv",std::ios::out);
        outFile << "time," << meas << ","
                << std::endl;
        outFile.close();
      }
      this->measLogs[meas].push_back(
       std::pair<std::string,std::string>(dateTime,valu)
      );
      if(this->measLogs[meas].size()>=BUFFER_SIZE) {
        this->writeMeas(meas);
        this->measLogs[meas].clear();
      }
    }
  }

  void Log::writeAll() const {
    for(const auto& e : this->evntLogs) {
      this->writeEvnt(e.first);
    }
    for(const auto& e : this->measLogs) {
      this->writeMeas(e.first);
    }
  }

  bool Log::isEnabled(const LogLevel& level) const {
    return this->levels&static_cast<uint8_t>(level);
  }

  void Log::writeEvnt(const std::string& evnt) const {
    std::ofstream outFile(this->dir+"evnt-"+evnt+".csv",std::ios::app);
    for(size_t i=0; i<this->evntLogs.at(evnt).size(); i++) {
      outFile << this->evntLogs.at(evnt).at(i) << ","
              << std::endl;
    }
    outFile.close();
  }

  void Log::writeMeas(const std::string& meas) const {
    std::ofstream outFile(this->dir+"meas-"+meas+".csv",std::ios::app);
    for(size_t i=0; i<this->measLogs.at(meas).size(); i++) {
      outFile << this->measLogs.at(meas).at(i).first << ","
              << this->measLogs.at(meas).at(i).second << ","
              << std::endl;
    }
    outFile.close();
  }
}
