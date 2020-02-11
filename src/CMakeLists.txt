#
# CMakeLists.txt
# (c) British Crown Owned Copyright 2019/AWE
#
# This file is part of TIO Browse
# Released under the BSD 3-clause license.
# For more details see license.txt
#

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${TyphonIO_INCLUDE_DIRS}
)

set(CORE_HEADER_FILES
    AboutInfo.h
    MainWindowMediator.h
    TIOTypeNames.h
)

set(CORE_SOURCE_FILES
    AboutInfo.cpp
    MainWindowMediator.cpp
    TIOTypeNames.cpp
)

add_library(tio_browse_core
    ${CORE_HEADER_FILES}
    ${CORE_SOURCE_FILES}
)

if (Compiler_warnings_enabled)
    target_compile_options(tio_browse_core PRIVATE "${WARNING_COMPILE_FLAGS}")
endif()

target_link_libraries(tio_browse_core
    ${TyphonIO_LIBRARIES}
    ${HDF5_LIBRARIES}
    ${ZLIB_LIBRARIES}
)

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
TIOVariable.h
)

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
TIOVariable.cpp
)

TIOInterface.cpp
TIOInterfaceGroup.cpp
TIOPolygon.cpp

TIOTypeNames.h
TIOTypeNames.cpp

if (TyphonIO_interfaces_enabled)
    LIST(APPEND DIALOG_HEADER_FILES
        TIOInterface.h
        TIOInterfaceGroup.h
        TIOPolygon.h
    )
    LIST(APPEND DIALOG_SOURCE_FILES
        TIOInterface.cpp
        TIOInterfaceGroup.cpp
        TIOPolygon.cpp
    )
endif()

# Build the app if required
if (Qt4_enabled OR Qt5_enabled)

    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)

    # Find the Qt library
    if (Qt4_enabled)
        find_package(Qt4 REQUIRED QtGui QtHelp)
    elseif(Qt5_enabled)
        find_package(Qt5Widgets REQUIRED)
    endif()

    add_library(tio_browse_qt
        ${DIALOG_HEADER_FILES}
        ${DIALOG_SOURCE_FILES}
    )

    if (TyphonIO_interface_enabled)
        target_compile_definitions(tio_browse_qt -DTYPHONIO_INTERFACES_ENABLED)
    endif()

    if (Compiler_warnings_enabled)
        target_compile_options(tio_browse_qt PRIVATE "${WARNING_COMPILE_FLAGS}")
    endif()

    target_link_libraries(tio_browse_qt tio_browse_core)

    if (Qt4_enabled)
        target_link_libraries(tio_browse_qt
            Qt4::QtHelp
            Qt4::QtGui
        )
    elseif(Qt5_enabled)
        target_link_libraries(tio_browse_qt
            Qt5::Widgets
        )
    endif()

    add_executable(tio_browse main.cpp)

    if (Compiler_warnings_enable)
        target_compile_options(tio_browse PRIVATE "${WARNING_COMPILE_FLAGS}")
    endif()

    # Use the QT_NO_DEBUG_OUTPUT flag to suppress output when using
    # the Qt qDebug class, for example:
    # target_compile_definitions(tio_browse PRIVATE "QT_NO_DEBUG_OUTPUT")

    target_link_libraries(tio_browse
        tio_browse_core
        tio_browse_qt
    )

    if (Qt4_enabled)
        target_link_libraries(tio_browse
            Qt4::QtHelp
            Qt4::QtGui
        )
    elseif(Qt5_enabled)
        target_link_libraries(tio_browse
            Qt5::Widgets
        )
    endif()

    install(TARGETS tio_browse DESTINATION bin)

endif()