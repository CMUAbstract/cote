# CMakeLists.txt
# CMakeLists.txt file
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cmake_minimum_required(VERSION 3.0)
project(AllParams)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
link_libraries(stdc++fs)
include_directories("../../../software/capacitor/include"
                    "../../../software/channel/include"
                    "../../../software/constants/include"
                    "../../../software/date-time/include"
                    "../../../software/ground-station/include"
                    "../../../software/isim/include"
                    "../../../software/log/include"
                    "../../../software/log-level/include"
                    "../../../software/receiver/include"
                    "../../../software/satellite/include"
                    "../../../software/sensor/include"
                    "../../../software/solar-array/include"
                    "../../../software/state-machine/include"
                    "../../../software/state-transition/include"
                    "../../../software/transmitter/include"
                    "../../../software/utilities/include"
                    "../../../dependencies/parse-json-0.1.0/software/array/include"
                    "../../../dependencies/parse-json-0.1.0/software/bool/include"
                    "../../../dependencies/parse-json-0.1.0/software/nul/include"
                    "../../../dependencies/parse-json-0.1.0/software/number/include"
                    "../../../dependencies/parse-json-0.1.0/software/object/include"
                    "../../../dependencies/parse-json-0.1.0/software/string/include"
                    "../../../dependencies/parse-json-0.1.0/software/type/include"
                    "../../../dependencies/parse-json-0.1.0/software/utilities/include"
                    "../../../dependencies/parse-json-0.1.0/software/value/include")
add_executable(all_params all-params.cpp
               ../../../software/capacitor/source/Capacitor.cpp
               ../../../software/channel/source/Channel.cpp
               ../../../software/date-time/source/DateTime.cpp
               ../../../software/ground-station/source/GroundStation.cpp
               ../../../software/log/source/Log.cpp
               ../../../software/receiver/source/Receiver.cpp
               ../../../software/satellite/source/Satellite.cpp
               ../../../software/sensor/source/Sensor.cpp
               ../../../software/solar-array/source/SolarArray.cpp
               ../../../software/state-machine/source/StateMachine.cpp
               ../../../software/state-transition/source/StateTransition.cpp
               ../../../software/transmitter/source/Transmitter.cpp
               ../../../software/utilities/source/utilities.cpp
               ../../../dependencies/parse-json-0.1.0/software/array/source/Array.cpp
               ../../../dependencies/parse-json-0.1.0/software/bool/source/Bool.cpp
               ../../../dependencies/parse-json-0.1.0/software/nul/source/Nul.cpp
               ../../../dependencies/parse-json-0.1.0/software/number/source/Number.cpp
               ../../../dependencies/parse-json-0.1.0/software/object/source/Object.cpp
               ../../../dependencies/parse-json-0.1.0/software/string/source/String.cpp
               ../../../dependencies/parse-json-0.1.0/software/utilities/source/json-util.cpp
               ../../../dependencies/parse-json-0.1.0/software/value/source/Value.cpp
)
