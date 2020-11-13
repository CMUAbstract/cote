# CMakeLists.txt
# CMakeLists.txt file
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cmake_minimum_required(VERSION 3.0)
project(TestNul)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
link_libraries(stdc++fs)
include_directories("../include"
                    "../../array/include"
                    "../../bool/include"
                    "../../number/include"
                    "../../object/include"
                    "../../string/include"
                    "../../type/include"
                    "../../utilities/include"
                    "../../value/include")
add_executable(test_nul test-nul.cpp
               ../source/Nul.cpp
               ../../array/source/Array.cpp
               ../../bool/source/Bool.cpp
               ../../number/source/Number.cpp
               ../../object/source/Object.cpp
               ../../string/source/String.cpp
               ../../utilities/source/utilities.cpp
               ../../value/source/Value.cpp)
