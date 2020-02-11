#
# CMakeLists.txt
# (c) British Crown Owned Copyright 2019/AWE
#
# This file is part of TIO Browse
# Released under the BSD 3-clause license.
# For more details see license.txt
#

cmake_minimum_required(VERSION 3.1)

project("TIO Browse")

enable_language(CXX C)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CPack)

option(tio_browse_enabled "Build TIO Browse" ON)
option(Qt4_enabled "Build with Qt4" OFF)
option(Qt5_enabled "Build with Qt5" OFF)
option(Example_data_enabled "Build example data" OFF)
option(Testing_enabled "Build testing" OFF)
option(Documentation_enabled
    "Build documentation using Doxygen" OFF)
option(Compiler_warning_enabled
    "Enable compiler warnings (not supported for all compilers" OFF)
option(TyphonIO_interfaces_enabled
    "Enable material interface support for TyphonIO (requires compatible TyphonIO)" OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(WARNING_COMPILE_FLAGS "-Wall")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(WARNING_COMPILER_FLAGS "-Wall" "-Wextra" "-Wpedantic" "-Wshadow"
                               "-Wold-style-cast" "-Woverloaded-virtual"
                               "-Wconversion")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    set(WARNING_COMPILE_FLAGS "-Wall")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(WARNING_COMPILE_FLAGS "/Wall")
else()
    message(STATUS "Compiler warnings are not available for "
                   "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
    set(WARNING_COMPILE_FLAGS "")
endif()

list(INSERT CMAKE_MODULE_PATH 0 "{PROJECT_SOURCE_DIR}/cmake/Modules")

if (Qt4_enabled AND Qt5_enabled)
    messaage(FATAL_ERROR "Can't enable Qt4 and Qt5 at the same time")
endif()

find_package(ZLIB REQUIRED)
find_package(HDF5 REQUIRED)
find_package(TyphonIO REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(tio_browse_enabled)
    add_subdirectory(src)
endif()

if(Example_data_enabled)
    add_subdirectory(data)
endif()

if(Testing_enabled)
    enable_testing()
    add_subdirectory(test)
endif()

if(Documentation_enabled)
    add_subdirectory(docs)
endif()
