# Name: Findparsical.cmake
#
# Description:
#   A cmake file allowing an easier time to find and use this library when using
#   CMake.

SET(SEARCH_DIRECTORIES
  /usr/local
)

# Finding the parsical headers.
find_path(PARSICAL_INCLUDE_DIR parsical.hpp
  PATH_SUFFIX include
  PATHS ${SEARCH_DIRECTORIES}
)

# Finding the parsical library.
find_library(PARSICAL_LIBRARY
  NAMES parsical
  PATH_SUFFIXES lib lib/static
  PATHS ${SERACH_DIRECTORIES}
)

set(PARSICAL_INCLUDE_DIRS ${PARSICAL_INCLUDE_DIR})
set(PARSICAL_LIBRARIES ${PARSICAL_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  PARSICAL
  REQUIRED_VARS CLIBGAME_INCLUDE_DIR CLIBGAME_LIBRARY
)
