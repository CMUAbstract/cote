# CMakeLists.txt
# CMakeLists.txt file
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cmake_minimum_required(VERSION 3.0)
project(TestUtilities)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
link_libraries(stdc++fs)
include_directories("../include"
                    "../../array/include"
                    "../../bool/include"
                    "../../nul/include"
                    "../../number/include"
                    "../../object/include"
                    "../../string/include"
                    "../../type/include"
                    "../../value/include")
add_executable(test_utilities test-utilities.cpp
               ../source/utilities.cpp
               ../../array/source/Array.cpp
               ../../bool/source/Bool.cpp
               ../../nul/source/Nul.cpp
               ../../number/source/Number.cpp
               ../../object/source/Object.cpp
               ../../string/source/String.cpp
               ../../value/source/Value.cpp)
