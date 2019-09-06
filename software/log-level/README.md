# LogLevel Enum Class

The `LogLevel` enum class enumerates the log levels. Log levels should be used
with the following guidance.
* NONE  (00000001): Suppress all logs
* FATAL (00000010): The application must be shutdown.
* ERROR (00000100): The algorithm cannot proceed, but the application could.
* WARN  (00001000): The application is automatically recovering from misuse.
* INFO  (00010000): Configuration parameters, service start/stop
* DEBUG (00100000): Diagnostics useful to IT, sysadmins, etc.
* TRACE (01000000): Used when tracing through to code during development
* ALL   (01111110): TRACE|DEBUG|INFO|WARN|ERROR|FATAL

## Directory Contents

* [include](include/LogLevel.hpp): Enum class definition
* [README.md](README.md): This document

## License

Written by Bradley Denby  
Other contributors: None

To the extent possible under law, the author(s) have dedicated all copyright and
related and neighboring rights to this work to the public domain worldwide. This
work is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication with this
work. If not, see <https://creativecommons.org/publicdomain/zero/1.0/>.
