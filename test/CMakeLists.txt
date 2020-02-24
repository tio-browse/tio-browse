#
# test/CMakeLists.txt
#
# (c) British Crown Owned Copyright 2019/AWE
#
# This file is part of TIO Browse
# Released under the BSD 3-clause license.
# For more details see license.txt
#

find_package(GTest REQUIRED)

set(TEST_HEADER_FILES MainWindowTestDouble.h)

set(TEST_SOURCE_FILES AboutInfo_test.cpp MainWindowMediator_test.cpp
                      MainWindowTestDouble.cpp test-runner.cpp)

add_executable(test-runner ${TEST_HEADER_FILES} ${TEST_SOURCE_FILES})

target_link_libraries(test-runner PRIVATE tio-browse-core TyphonIO::typhonio
                                          GTest::GTest GTest::Main)

target_include_directories(test-runner PRIVATE "${PROJECT_SOURCE_DIR}/src")

# Qt tests
gtest_discover_tests(test-runner)

# Build the app if required
if(Qt4_enabled OR Qt5_enabled)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTOUIC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  set(TEST_QT_HEADER_FILES MockTIOTreeItem.h TIOTreeItemTestFixture.h)

  set(TEST_QT_SOURCE_FILES
      DataArray_test.cpp
      DataTableModel_test.cpp
      TIOFile_test.cpp
      TIOMaterial_test.cpp
      TIOMaterialChunk_test.cpp
      TIOMaterialChunkGroup_test.cpp
      TIOMesh_test.cpp
      TIOMeshChunk_test.cpp
      TIOMeshChunkGroup_test.cpp
      TIOQuantChunk_test.cpp
      TIOQuantChunkGroup_test.cpp
      TIOQuant_test.cpp
      TIOState_test.cpp
      TIOTreeItem_test.cpp
      TIOTreeItemTestFixture.cpp
      TIOTreeModel_test.cpp
      TIOVargroup_test.cpp
      TIOVariable_test.cpp
      test-runner-qt.cpp)

  if(TyphonIO_interfaces_enabled)
    set(TEST_QT_SOURCE_FILES ${TEST_QT_SOURCE_FILES} TIOInterfaceGroup_test.cpp
                             TIOInterface_test.cpp TIOPolygon_test.cpp)
  endif()

  # Find the Qt library
  if(Qt4_enabled)
    find_package(Qt4 REQUIRED COMPONENTS QtGui QtTest)
  elseif(Qt5_enabled)
    find_package(Qt5 REQUIRED COMPONENTS Widgets Test)
  endif()

  add_executable(test-runner-qt ${TEST_QT_HEADER_FILES} ${TEST_QT_SOURCE_FILES})

  target_link_libraries(test-runner-qt PRIVATE tio-browse-core tio-browse-qt
                                               TyphonIO::typhonio)

  if(TyphonIO_interfaces_enabled)
    target_compile_definitions(test-runner-qt -DTYPHONIO_INTERFACES_ENABLED)
  endif()

  target_link_libraries(test-runner-qt PRIVATE GTest::GTest GTest::Main)

  # Link Qt
  if(Qt4_enabled)
    target_link_libraries(test-runner-qt PRIVATE Qt4::QtGui Qt4::QtTest)
  elseif(Qt5_enabled)
    target_link_libraries(test-runner-qt PRIVATE Qt5::Widgets Qt5::Test)
  endif()

  target_include_directories(test-runner-qt PRIVATE "${PROJECT_SOURCE_DIR}/src")

  gtest_discover_tests(test-runner-qt)

  set(TEST_QT_GUI_SOURCE_FILES DataDialog_test.cpp)

  add_executable(test-runner-qt-gui ${TEST_QT_GUI_SOURCE_FILES})

  target_link_libraries(
    test-runner-qt-gui PRIVATE tio-browse-core tio-browse-qt TyphonIO::typhonio)

  target_include_directories(test-runner-qt-gui
                             PRIVATE "${PROJECT_SOURCE_DIR}/src")

  if(Qt4_enabled)
    target_link_libraries(test-runner-qt-gui PRIVATE Qt4::QtGui Qt4::QtTest)
  elseif(Qt5_enabled)
    target_link_libraries(test-runner-qt-gui PRIVATE Qt5::Widgets Qt5::Test)
  endif()

  add_test(AllQtGuiTests test-runner-qt-gui)

endif()
