# CMakeLists.txt

# (c) British Crown Owned Copyright 2020/AWE
#
# This file is part of TIO Browse
# Released under the BSD 3-clause license.
# For more details see license.txt

cmake_minimum_required(VERSION 3.16)

project("tio-browse")

enable_language(CXX C)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CPack)
option(Python_enabled "Build with a consonle using Python and Numpy" ON)
option(Qt4_enabled "Build with Qt4" OFF)
option(Qt5_enabled "Build with Qt5" OFF)
option(Example_data_enabled "Build example data" OFF)
option(Testing_enabled "Build testing" OFF)
option(Documentation_enabled "Build documentation using Doxygen" OFF)
option(Compiler_warning_enabled
       "Enable compiler warnings (not supported for all compilers" OFF)
option(
  TyphonIO_interfaces_enabled
  "Enable material interface support for TyphonIO (requires compatible TyphonIO)"
  OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(WARNING_COMPILE_FLAGS "-Wall")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(WARNING_COMPILER_FLAGS
      "-Wall"
      "-Wextra"
      "-Wpedantic"
      "-Wshadow"
      "-Wold-style-cast"
      "-Woverloaded-virtual"
      "-Wconversion")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  set(WARNING_COMPILE_FLAGS "-Wall")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(WARNING_COMPILE_FLAGS "/Wall")
else()
  message(STATUS "Compiler warnings are not available for "
                 "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
  set(WARNING_COMPILE_FLAGS "")
endif()

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/Modules")

if(Qt4_enabled AND Qt5_enabled)
  message(FATAL_ERROR "Can't enable Qt4 and Qt5 at the same time")
endif()

find_package(ZLIB REQUIRED)
find_package(HDF5 REQUIRED)
find_package(TyphonIO REQUIRED)

# Find the Qt library
if(Qt4_enabled)
  find_package(Qt4 REQUIRED COMPONENTS QtGui QtHelp)
elseif(Qt5_enabled)
  find_package(Qt5 REQUIRED COMPONENTS Widgets)
endif()

if(Qt4_enabled OR Qt5_enabled)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTOUIC ON)
  set(CMAKE_AUTORCC ON)
  ADD_DEFINITIONS(-DQT_NO_KEYWORDS)
endif()


if(Python_enabled)
  ADD_DEFINITIONS(-DCONSOLE)
  find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
endif

add_subdirectory(src)

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
