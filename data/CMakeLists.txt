#
# data/CMakeLists.txt
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

include_directories(${TyphonIO_INCLUDE_DIRS})

# List of examples to build
set(example_names
    "ex_colinear_mesh"
    "ex_noncolinear_mesh"
    "ex_unstructured_mesh"
    "ex_vargroup"
)

if(TyphonIO_interfaces_enabled)
    list(APPEND example_names "ex_interface_mesh")
endif()

# Add executable for each example
foreach(example ${example_names})
    add_executable(${example} ${example}.cpp)
    target_link_libraries(${example}
        ${TyphonIO_LIBRARIES}
        ${HDF5_LIBRARIES}
        ${ZLIB_LIBRARIES}
    )
endforeach()

# Create target for each TyphonIO example file
foreach(example ${example_names})
    add_custom_command(OUTPUT "${example}.h5"
        COMMAND "${example}"
        DEPENDS "${example}"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/data
    )
endforeach()

APPEND_LIST_ITEMS("${example_names}" ".h5" example_names_h5)
message(STATUS "example_names_h5 ${example_names_h5}")

# Add a target to generate all the TyphonIO example_files
add_custom_target(example_data DEPENDS "${example_names_h5}"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/data
)
