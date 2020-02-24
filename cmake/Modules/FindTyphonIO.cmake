#.rst:
# FindTyphonIO
# ------------
#
# (c) British Crown Owned Copyright 2020/AWE
#
# This file is part of TIO Browse
# Released under the BSD 3-clause license.
# For more details see license.txt
#
#
# Find TyphonIO, a library for reading and writing TyphonIO format data files.
#
# Result variables
# ^^^^^^^^^^^^^^^^
#
# This module will define the following variables:
#
#   ``TYPHONIO_FOUND``
#   Found the TyphonIO library
#   ``TyphonIO_INCLUDE_DIRS``
#   Paths of TyphonIO include directories
#   ``TyphonIO_LIBRARIES``
#   The TyphonIO libraries
#
# Example usage
# ^^^^^^^^^^^^^
#
# ::
#
#     find_package(TyphonIO REQUIRED)
#
#     add_executable(foo foo.cpp)
#     target_link_libraries(foo PRIVATE TyphonIO::typhonio)

find_path(TyphonIO_INCLUDE_DIR typhonio.h PATH_SUFFIXES include)
mark_as_advanced(TyphonIO_INCLUDE_DIR)

find_library(
  TyphonIO_C_LIBRARY
  NAMES typhonio
  PATH_SUFFIXES lib)
mark_as_advanced(TyphonIO_C_LIBRARY)

include(CMakeFindDependencyMacro)
find_dependency(HDF5 REQUIRED)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TyphonIO DEFAULT_MSG TyphonIO_INCLUDE_DIR
                                  TyphonIO_C_LIBRARY)

if(TYPHONIO_FOUND)
  set(TyphonIO_LIBRARIES "${TyphonIO_C_LIBRARY}")
  set(TyphonIO_INCLUDE_DIRS "${TyphonIO_INCLUDE_DIR}")

  add_library(TyphonIO::typhonio SHARED IMPORTED)
  set_property(TARGET TyphonIO::typhonio PROPERTY IMPORTED_LOCATION
                                                  "${TyphonIO_LIBRARIES}")
  target_link_libraries(TyphonIO::typhonio INTERFACE "${HDF5_LIBRARIES}")
  target_include_directories(TyphonIO::typhonio
                             INTERFACE "${TyphonIO_INCLUDE_DIRS}")
endif()
