# CMakeLists.txt
# CMakeLists.txt file
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cmake_minimum_required(VERSION 3.0)
project(TestStateMachine)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
include_directories("../include"
                    "../../log/include"
                    "../../log-level/include"
                    "../../state-transition/include")
add_executable(test_state_machine test-state-machine.cpp
               ../source/StateMachine.cpp
               ../../log/source/Log.cpp
               ../../state-transition/source/StateTransition.cpp)
