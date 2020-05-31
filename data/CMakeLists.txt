#
# data/CMakeLists.txt
#
# (c) British Crown Owned Copyright 2019/AWE
#
# This file is part of TIO Browse
# Released under the BSD 3-clause license.
# For more details see license.txt
#

# Macro to append suffix to each list item
macro(APPEND_LIST_ITEMS list_1 suffix_to_append list_2)
  set(${list_2} "")
  foreach(list_item ${list_1})
    list(APPEND ${list_2} "${list_item}.h5")
  endforeach(list_item list_1)
endmacro()

# List of examples to build
set(example_names "ex_colinear_mesh" "ex_noncolinear_mesh"
                  "ex_unstructured_mesh" "ex_vargroup")

if(TyphonIO_interfaces_enabled)
  list(APPEND example_names "ex_interface_mesh")
endif()

# Add executable for each example
foreach(example ${example_names})
  add_executable(${example} ${example}.cpp)
  target_link_libraries(${example} PRIVATE TyphonIO::typhonio)
endforeach()

# Create target for each TyphonIO example file
foreach(example ${example_names})
  add_custom_command(
    OUTPUT "${example}.h5"
    COMMAND "${example}"
    DEPENDS "${example}"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/data)
endforeach()

append_list_items("${example_names}" ".h5" example_names_h5)
message(STATUS "example_names_h5 ${example_names_h5}")

# tio-generator
add_executable(tio-generator tio_generator.C)
target_link_libraries(tio-generator PRIVATE TyphonIO::typhonio)

add_custom_command(
  OUTPUT 3d_chunk_v1.h5 3d_point_v1.h5
  COMMAND tio-generator
  DEPENDS tio-generator
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/data)

# Add a target to generate all the TyphonIO example_files
add_custom_target(
  example_data
  DEPENDS "${example_names_h5}" 3d_chunk_v1.h5 3d_point_v1.h5
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/data)
