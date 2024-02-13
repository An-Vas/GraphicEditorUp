# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\GraphicEditorUp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\GraphicEditorUp_autogen.dir\\ParseCache.txt"
  "GraphicEditorUp_autogen"
  )
endif()
