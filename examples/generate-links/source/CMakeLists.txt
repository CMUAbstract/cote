# CMakeLists.txt
# CMakeLists.txt file
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cmake_minimum_required(VERSION 3.0)
project(GenerateLinks)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
include_directories("../../../software/constants/include"
                    "../../../software/date-time/include"
                    "../../../software/ground-station/include"
                    "../../../software/isim/include"
                    "../../../software/log/include"
                    "../../../software/log-level/include"
                    "../../../software/satellite/include"
                    "../../../software/utilities/include")
add_executable(generate_links generate-links.cpp
               ../../../software/date-time/source/DateTime.cpp
               ../../../software/ground-station/source/GroundStation.cpp
               ../../../software/log/source/Log.cpp
               ../../../software/satellite/source/Satellite.cpp
               ../../../software/utilities/source/utilities.cpp)
