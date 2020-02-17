#
# docs/CMakeLists.txt
#
# (c) British Crown Owned Copyright 2019/AWE
#
# This file is part of TIO Browse
# Released under the BSD 3-clause license.
# For more details see license.txt
#

find_package(Doxygen REQUIRED)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
               ${cmake_current_binary_dir}/Doxyfile @ONLY)
add_custom_target(
  docs
  ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating documentation with Doxygen"
  VERBATIM)
