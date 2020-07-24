# CMakeLists.txt
# CMakeLists.txt file
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cmake_minimum_required(VERSION 3.0)
project(DynamicTiling)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
link_libraries(stdc++fs)
include_directories("../../../software/constants/include"
                    "../../../software/date-time/include"
                    "../../../software/isim/include"
                    "../../../software/log/include"
                    "../../../software/log-level/include"
                    "../../../software/satellite/include"
                    "../../../software/sensor/include"
                    "../../../software/utilities/include")
add_executable(dynamic_tiling dynamic-tiling.cpp
               ../../../software/date-time/source/DateTime.cpp
               ../../../software/log/source/Log.cpp
               ../../../software/satellite/source/Satellite.cpp
               ../../../software/sensor/source/Sensor.cpp
               ../../../software/utilities/source/utilities.cpp)
