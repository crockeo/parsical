cmake_minimum_required(VERSION 3.1)
project(parsical)

# Setting CMake / compiler flags.
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build")
set(CMAKE_CXX_FLAGS "-Wall -O2 -std=c++11 ${CMAKE_CXX_FLAGS}")

# Setting up the library.
set(SOURCES
  src/parsical/parsestream.cpp
  src/parsical/parseerror.cpp
)

add_library(parsical STATIC ${SOURCES})

# Setting up the test suite.
set(TEST_SOURCES
  src/test/setup.cpp
  src/test/main.cpp
)

add_executable(parsical-test ${TEST_SOURCES})
target_link_libraries(parsical-test parsical)

# Installation instructions.
install(TARGETS parsical
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

install(DIRECTORY src/
        DESTINATION include
        FILES_MATCHING PATTERN *.hpp)

install(FILES src/Findparsical.cmake
        DESTINATION "${CMAKE_ROOT}/Modules")
