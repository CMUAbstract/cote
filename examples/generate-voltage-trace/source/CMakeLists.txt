# CMakeLists.txt
# CMakeLists.txt file
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cmake_minimum_required(VERSION 3.0)
project(GenerateVoltageTrace)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
link_libraries(stdc++fs)
include_directories("../../../software/capacitor/include"
                    "../../../software/constants/include"
                    "../../../software/date-time/include"
                    "../../../software/isim/include"
                    "../../../software/log/include"
                    "../../../software/log-level/include"
                    "../../../software/satellite/include"
                    "../../../software/solar-array/include"
                    "../../../software/state-machine/include"
                    "../../../software/state-transition/include"
                    "../../../software/utilities/include")
add_executable(generate_voltage_trace generate-voltage-trace.cpp
               ../../../software/capacitor/source/Capacitor.cpp
               ../../../software/date-time/source/DateTime.cpp
               ../../../software/log/source/Log.cpp
               ../../../software/satellite/source/Satellite.cpp
               ../../../software/solar-array/source/SolarArray.cpp
               ../../../software/state-machine/source/StateMachine.cpp
               ../../../software/state-transition/source/StateTransition.cpp
               ../../../software/utilities/source/utilities.cpp)
