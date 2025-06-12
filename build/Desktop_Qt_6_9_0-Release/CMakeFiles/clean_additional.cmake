# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles/TicTacTueServer_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/TicTacTueServer_autogen.dir/ParseCache.txt"
  "TicTacTueServer_autogen"
  )
endif()
