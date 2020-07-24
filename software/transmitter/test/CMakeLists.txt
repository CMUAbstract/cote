# CMakeLists.txt
# CMakeLists.txt file
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cmake_minimum_required(VERSION 3.0)
project(TestTransmitter)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
include_directories("../include"
                    "../../constants/include"
                    "../../date-time/include"
                    "../../isim/include"
                    "../../log/include"
                    "../../log-level/include"
                    "../../utilities/include")
add_executable(test_transmitter test-transmitter.cpp
               ../source/Transmitter.cpp
               ../../date-time/source/DateTime.cpp
               ../../log/source/Log.cpp
               ../../utilities/source/utilities.cpp)
