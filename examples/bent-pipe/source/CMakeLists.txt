# CMakeLists.txt
# CMakeLists.txt file
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cmake_minimum_required(VERSION 3.0)
project(BentPipe)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
link_libraries(stdc++fs)
include_directories("../../../software/channel/include"
                    "../../../software/constants/include"
                    "../../../software/date-time/include"
                    "../../../software/ground-station/include"
                    "../../../software/isim/include"
                    "../../../software/log/include"
                    "../../../software/log-level/include"
                    "../../../software/receiver/include"
                    "../../../software/satellite/include"
                    "../../../software/sensor/include"
                    "../../../software/transmitter/include"
                    "../../../software/utilities/include")
add_executable(bent_pipe bent-pipe.cpp
               ../../../software/channel/source/Channel.cpp
               ../../../software/date-time/source/DateTime.cpp
               ../../../software/ground-station/source/GroundStation.cpp
               ../../../software/log/source/Log.cpp
               ../../../software/receiver/source/Receiver.cpp
               ../../../software/satellite/source/Satellite.cpp
               ../../../software/sensor/source/Sensor.cpp
               ../../../software/transmitter/source/Transmitter.cpp
               ../../../software/utilities/source/utilities.cpp)
