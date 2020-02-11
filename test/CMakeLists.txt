#
# CMakeLists.txt
# (c) British Crown Owned Copyright 2019/AWE
#
# This file is part of TIO Browse
# Released under the BSD 3-clause license.
# For more details see license.txt
#

find_package(GTest REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR on)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${GTEST_INCLUDE_DIRS}
    ${TyphonIO_INCLUDE_DIRS}
)

set(TEST_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindowTestDouble.h
)

set(TEST_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/AboutInfo_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindowMediator_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindowTestDouble.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_runner.cpp
)

add_executable(test_runner
    ${TEST_HEADER_FILES}
    ${TEST_SOURCE_FILES}
)

target_link_libraries(test_runner
    tiotool_core
    ${TyphonIO_LIBRARIES}
    ${HDF5_LIBRARIES}
)

# Link to Google Test libraries
if("${CMAKE_VERSION}" VERSION_GREATER 3.5)
    target_link_libraries(test_runner
        GTest::GTest GTest::Main
    )
else()
    target_link_libraries(test_runner
         ${GTEST_LIBRARIES} pthread
    )
endif()

# Qt tests
add_test(AllGoogleTests test_runner)

# Build the app if required
if(Qt4_enabled or Qt5_enabled)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)

    set(TEST_QT_HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/MockTIOTreeItem.h
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOTreeItemTestFixture.h
    )

    set(TEST_QT_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/DataArray_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/DataTableModel_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOFile_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOMaterial_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOMaterialChunk_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOMaterialChunkGroup_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOMesh_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOMeshChunk_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOMeshChunkGroup_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOQuantChunk_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOQuantChunkGroup_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOQuant_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOState_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOTreeItem_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOTreeItemTestFixture.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOTreeModel_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOVargroup_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TIOVariable_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_runner_qt.cpp
    )

    if(TyphonIO_interfaces_enabled)
        set(TEST_QT_SOURCE_FILES ${TEST_QT_SOURCE_FILES}
            ${CMAKE_CURRENT_SOURCE_DIR}/TIOInterfaceGroup_test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/TIOInterface_test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/TIOPolygon_test.cpp
    endif()

    # Find the Qt library
    if (Qt4_enabled)
        find_package(Qt4 REQUIRED QtGui QtTest)
    elseif(Qt5_enabled)
        find_pacakge(Qt5Widgets REQUIRED)
        find_package(Qt5Test REQUIRED)
    endif()

    add_executable(test_runner_qt
        ${TEST_QT_HEADER_FILES}
        ${TEST_QT_SOURCE_FILES}
    )

    target_link_libraries(test_runner_qt
        PRIVATE tiotool_core tiotool_qt ${TyphonIO_LIBRARIES} ${HDF5_LIBRARIES}
    )

    if(TyphonIO_interfaces_enabled)
        target_compile_definitions(test_runner_qt -DTYPHONIO_INTERFACES_ENABLED)
    endif()

    # Link to Google Test libraries
    if("${CMAKE_VERSION}" VERSION_GREATER 3.5)
        target_link_libraries(test_runner_qt
            GTest::GTest GTest::Main
        )
    else()
        target_link_libraries(test_runner_qt
             ${GTEST_LIBRARIES} pthread
        )
    endif()

    # Link Qt
    if (Qt4_enabled)
        target_link_libraries(test_runner_qt PRIVATE Qt4::QtGui Qt4::QtTest)
    elseif(Qt5_enabled)
        target_link_libraries(test_runner_qt PRIVATE Qt5::Widgets Qt5::Test)
    endif()

    add_test(AllQtTests test_runner_qt)

    set(TEST_QT_GUI_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/DataDialog_test.cpp
    )

    add_executable(test_runner_qt_gui ${TEST_QT_GUI_SOURCE_FILES})

    target_link_libraries(test_runner_qt_gui
        tiotool_core tiotool_qt ${TyphonIO_LIBRARIES} ${HDF5_LIBRARIES}
    )

    if (Qt4_enabled)
        target_link_libraries(test_runner_qt_gui Qt4::QtGui Qt4::QtTest)
    elseif (Qt5_enabled)
        target_link_libraries(test_runner_qt_gui Qt4::Widgets Qt5::Test)
    endif()

    add_test(AllQtGuiTests test_runner_qt_gui)

endif()
