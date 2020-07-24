// Log.hpp
// Log class header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef COTE_LOG_HPP
#define COTE_LOG_HPP

// Standard library
#include <cstdint>      // uint8_t, uint16_t
#include <map>          // map
#include <string>       // string
#include <utility>      // pair
#include <vector>       // vector

// cote library
#include <LogLevel.hpp> // LogLevel

namespace cote {
  class Log {
  public:
    // levels: a vector of LogLevels to be heeded; all others will be ignored
    // dir: directory for log files
    Log(const std::vector<LogLevel>& levels, const std::string& dir);
    // level: log level of this log
    // dateTime: ISO 8601 date and time or similar
    // evnt: event name
    void evnt(
     const LogLevel& level, const std::string& dateTime, const std::string& evnt
    );
    // level: log level of this log
    // dateTime: ISO 8601 date and time or similar
    // meas: measurement name
    // valu: measurement value
    void meas(
     const LogLevel& level, const std::string& dateTime,
     const std::string& meas, const std::string& valu
    );
    // Appends all logs to their corresponding log files
    void writeAll() const;
  private:
    // Maximum number of logs before appending to corresponding map key file
    static const uint16_t BUFFER_SIZE = 65535;
    // Check whether the given log level is enabled
    bool isEnabled(const LogLevel& level) const;
    // Appends specified events to corresponding log file
    void writeEvnt(const std::string& evnt) const;
    // Appends specified measurements to corresponding log file
    void writeMeas(const std::string& meas) const;
    // Flag indicating enabled log levels (see LogLevel enum class)
    uint8_t levels;
    // Default directory for log files
    std::string dir;
    // Event log map: (event, {times})
    std::map<
     std::string,std::vector<std::string>
    > evntLogs;
    // Measurement log map: (measurement, {time, value})
    std::map<
     std::string,std::vector<std::pair<std::string,std::string>>
    > measLogs;
  };
}

#endif
