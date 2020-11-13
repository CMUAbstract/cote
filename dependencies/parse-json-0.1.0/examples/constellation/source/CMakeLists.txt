# CMakeLists.txt
# CMakeLists.txt file
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cmake_minimum_required(VERSION 3.0)
project(TestParseConstellation)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
link_libraries(stdc++fs)
include_directories("../../../software/array/include"
                    "../../../software/bool/include"
                    "../../../software/nul/include"
                    "../../../software/number/include"
                    "../../../software/object/include"
                    "../../../software/string/include"
                    "../../../software/type/include"
                    "../../../software/utilities/include"
                    "../../../software/value/include")
add_executable(test_parse_constellation test-parse-constellation.cpp
               ../../../software/array/source/Array.cpp
               ../../../software/bool/source/Bool.cpp
               ../../../software/nul/source/Nul.cpp
               ../../../software/number/source/Number.cpp
               ../../../software/object/source/Object.cpp
               ../../../software/string/source/String.cpp
               ../../../software/utilities/source/utilities.cpp
               ../../../software/value/source/Value.cpp)
