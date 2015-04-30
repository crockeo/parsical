cmake_minimum_required(VERSION 3.1)
project(parsical)

# Setting CMake / compiler flags.
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build")
set(CMAKE_CXX_FLAGS "-Wall -O2 -std=c++11 ${CMAKE_CXX_FLAGS}")

set(SOURCES
  src/parsical/parsestream.cpp
)

# Adding the library declaration.
add_library(parsical STATIC ${SOURCES})

# Installation instructions.
install(TARGETS parsical
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

install(DIRECTORY src/
        DESTINATION include
        FILES_MATCHING PATTERN *.hpp)

install(FILES src/Findparsical.cmake
        DESTINATION "${CMAKE_ROOT}/Modules")
