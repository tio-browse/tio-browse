#
# src/CMakeLists.txt
#
# (c) British Crown Owned Copyright 2019/AWE
#
# This file is part of TIO Browse
# Released under the BSD 3-clause license.
# For more details see license.txt
#

set(CORE_HEADER_FILES AboutInfo.h MainWindowMediator.h TIOTypeNames.h)

set(CORE_SOURCE_FILES AboutInfo.cpp MainWindowMediator.cpp TIOTypeNames.cpp)

add_library(tio-browse-core ${CORE_HEADER_FILES} ${CORE_SOURCE_FILES})

if(Compiler_warnings_enabled)
  target_compile_options(tio-browse-core PRIVATE "${WARNING_COMPILE_FLAGS}")
endif()

target_link_libraries(tio-browse-core TyphonIO::typhonio ${HDF5_LIBRARIES}
                      ${ZLIB_LIBRARIES})

set(DIALOG_HEADER_FILES
    AboutDialog.h
    AboutInfo.h
    DataArray.h
    DataDialog.h
    DataTableModel.h
    ErrorDialog.h
    MainWindow.h
    TIODataItem.h
    TIOException.h
    TIOFile.h
    TIOInfoGroup.h
    TIOInfoItem.h
    TIOMaterial.h
    TIOMaterialChunk.h
    TIOMaterialChunkGroup.h
    TIOMesh.h
    TIOMeshChunk.h
    TIOMeshChunkGroup.h
    TIOQuant.h
    TIOQuantChunk.h
    TIOQuantChunkGroup.h
    TIOQuantGroup.h
    TIORootItem.h
    TIOState.h
    TIOTreeItem.h
    TIOTreeModel.h
    TIOVargroup.h
    TIOVariable.h)

set(DIALOG_SOURCE_FILES
    AboutDialog.cpp
    DataArray.cpp
    DataDialog.cpp
    DataTableModel.cpp
    ErrorDialog.cpp
    MainWindow.cpp
    TIODataItem.cpp
    TIOException.cpp
    TIOFile.cpp
    TIOInfoGroup.cpp
    TIOInfoItem.cpp
    TIOMaterial.cpp
    TIOMaterialChunk.cpp
    TIOMaterialChunkGroup.cpp
    TIOMesh.cpp
    TIOMeshChunk.cpp
    TIOMeshChunkGroup.cpp
    TIOQuant.cpp
    TIOQuantChunk.cpp
    TIOQuantChunkGroup.cpp
    TIOQuantGroup.cpp
    TIORootItem.cpp
    TIOState.cpp
    TIOTreeItem.cpp
    TIOTreeModel.cpp
    TIOVargroup.cpp
    TIOVariable.cpp)

if(TyphonIO_interfaces_enabled)
  list(APPEND DIALOG_HEADER_FILES TIOInterface.h TIOInterfaceGroup.h
       TIOPolygon.h)
  list(APPEND DIALOG_SOURCE_FILES TIOInterface.cpp TIOInterfaceGroup.cpp
       TIOPolygon.cpp)
endif()

if(Python_enabled)
  list(APPEND DIALOG_HEADER_FILES
    ${qtconsole_SOURCE_DIR}/include/qconsole.h
    ${qtconsole_SOURCE_DIR}/include/qpyconsole.h 
    ConvTIOtoPyTypes.h)
  list(APPEND DIALOG_SOURCE_FILES
    ${qtconsole_SOURCE_DIR}/src/qconsole.cpp
    ${qtconsole_SOURCE_DIR}/src/qpyconsole.cpp
    ConvTIOtoPyTypes.cpp)
endif()

# Build the app if required
if(Qt4_enabled OR Qt5_enabled)

  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTOUIC ON)
  set(CMAKE_AUTORCC ON)

  add_library(tio-browse-qt ${DIALOG_HEADER_FILES} ${DIALOG_SOURCE_FILES})

  if(TyphonIO_interface_enabled)
    target_compile_definitions(tio-browse-qt -DTYPHONIO_INTERFACES_ENABLED)
  endif()

  if(Compiler_warnings_enabled)
    target_compile_options(tio-browse-qt PRIVATE "${WARNING_COMPILE_FLAGS}")
  endif()

  target_link_libraries(tio-browse-qt tio-browse-core)

  if(Qt4_enabled)
    target_link_libraries(tio-browse-qt Qt4::QtHelp Qt4::QtGui)
  elseif(Qt5_enabled)
    target_link_libraries(tio-browse-qt Qt5::Widgets)
  endif()

  if(Python_enabled)
    target_include_directories(tio-browse-qt PUBLIC
      ${qtconsole_SOURCE_DIR}/include)
    target_compile_definitions(tio-browse-qt PRIVATE CONSOLE QT_NO_KEYWORDS)
    target_link_libraries(tio-browse-qt Python3::Python Python3::NumPy)
  endif() 

  add_executable(tio-browse main.cpp)

  if(Compiler_warnings_enable)
    target_compile_options(tio-browse PRIVATE "${WARNING_COMPILE_FLAGS}")
  endif()

    # Use the QT_NO_DEBUG_OUTPUT flag to suppress output when using
    # the Qt qDebug class, for example:
    # target_compile_definitions(tio-browse PRIVATE "QT_NO_DEBUG_OUTPUT")

  target_link_libraries(tio-browse tio-browse-core tio-browse-qt)

  if(Qt4_enabled)
    target_link_libraries(tio-browse Qt4::QtHelp Qt4::QtGui)
  elseif(Qt5_enabled)
    target_link_libraries(tio-browse Qt5::Widgets)
  endif()

  if(Python_enabled)
    target_include_directories(tio-browse-qt PUBLIC
      ${qtconsole_SOURCE_DIR}/include)
    target_compile_definitions(tio-browse PRIVATE CONSOLE QT_NO_KEYWORDS)
    target_link_libraries(tio-browse Python3::Python Python3::NumPy)
  endif()  

  install(TARGETS tio-browse DESTINATION bin)

endif()
