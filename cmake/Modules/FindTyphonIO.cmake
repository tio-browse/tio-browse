#.rst:
# FindTyphonIO
# ------------
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
#     target_include_directories(foo PRIVATE ${TyphonIO_INCLUDE_DIRS})
#     target_link_libraries(foo PRIVATE ${TyphonIO_LIBRARIES})

find_path(TyphonIO-INCLUDE_DIR typhonio.h
    PATH_SUFFIXES
        include
)
mark_as_advanced(TyphonIO_INCLUDE_DIR)

find_library(TyphonIO_C_LIBRARY
    NAMES typhonio
    PATH_SUFFIXES
        lib
)
mark_as_advanced(TyphonIO_C_LIBRARY)

include(CMakeFindDependencyMacro)
find_dependency(HDF5 REQUIRED)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TyphonIO TyphonIO_INCLUDE_DIR TyphonIO_C_LIBRARY)

if(TYPHONIO_FOUND)
    set(TyphonIO_LIBRARIES ${TyphonIO_C_LIBRARY})
    set(TyphonIO_INCLUDE_DIRS ${TyphonIO_INCLUDE_DIR})
endif()
