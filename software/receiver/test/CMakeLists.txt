# CMakeLists.txt
# CMakeLists.txt file
#
# Written by Bradley Denby
# Other contributors: None
#
# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication with this
# software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

cmake_minimum_required(VERSION 3.0)
project(TestReceiver)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
include_directories("../include"
                    "../../constants/include"
                    "../../date-time/include"
                    "../../isim/include"
                    "../../log/include"
                    "../../log-level/include"
                    "../../utilities/include")
add_executable(test_receiver test-receiver.cpp
               ../source/Receiver.cpp
               ../../date-time/source/DateTime.cpp
               ../../log/source/Log.cpp
               ../../utilities/source/utilities.cpp)
