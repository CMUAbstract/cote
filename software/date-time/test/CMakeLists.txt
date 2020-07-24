# CMakeLists.txt
# CMakeLists.txt file
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cmake_minimum_required(VERSION 3.0)
project(TestDateTime)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
include_directories("../include"
                    "../../constants/include"
                    "../../isim/include"
                    "../../log/include"
                    "../../log-level/include"
                    "../../utilities/include")
add_executable(test_date_time test-date-time.cpp
               ../source/DateTime.cpp
               ../../log/source/Log.cpp
               ../../utilities/source/utilities.cpp)
