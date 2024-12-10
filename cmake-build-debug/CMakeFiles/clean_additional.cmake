# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\OneDay_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\OneDay_autogen.dir\\ParseCache.txt"
  "OneDay_autogen"
  )
endif()
