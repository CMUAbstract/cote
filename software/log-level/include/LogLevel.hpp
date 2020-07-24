// LogLevel.hpp
// Log level header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

#ifndef COTE_LOG_LEVEL_HPP
#define COTE_LOG_LEVEL_HPP

// Standard library
#include <cstdint> // uint8_t

// cote library
// None

// Log level guidance, modified from https://stackoverflow.com/a/2031209
// - NONE  (00000001): Suppress all logs
// - FATAL (00000010): The application must be shutdown.
// - ERROR (00000100): The algorithm cannot proceed, but the application could.
// - WARN  (00001000): The application is automatically recovering from misuse.
// - INFO  (00010000): Configuration parameters, service start/stop
// - DEBUG (00100000): Diagnostics useful to IT, sysadmins, etc.
// - TRACE (01000000): Used when tracing through to code during development
// - ALL   (01111110): TRACE|DEBUG|INFO|WARN|ERROR|FATAL

namespace cote {
  enum class LogLevel : uint8_t {
    NONE  =   1,
    FATAL =   2,
    ERROR =   4,
    WARN  =   8,
    INFO  =  16,
    DEBUG =  32,
    TRACE =  64,
    ALL   = 126
  };
}

#endif
