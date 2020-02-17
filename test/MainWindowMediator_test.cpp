//
// test/MainWindowMediator_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/MainWindowMediator.h"

#include <gtest/gtest.h>

#include "test/MainWindowTestDouble.h"

TEST(MainWindowMediator, CheckAboutInfo) {
  MainWindowTestDouble mainWindow;
  MainWindowMediator mediator(&mainWindow);

  AboutInfo aboutInfo = mediator.getAboutInfo();

  EXPECT_EQ("TIO browse", aboutInfo.getAppName());
  EXPECT_EQ("0.0.1", aboutInfo.getAppVersion());
  EXPECT_EQ(
      "TIO browse is a viewer for exploring the structure of "
      "TyphonIO files",
      aboutInfo.getAppDescription());
  EXPECT_EQ("Jim Eliot", aboutInfo.getCustodianName());
  EXPECT_EQ("jim.eliot@awe.co.uk", aboutInfo.getCustodianEmail());
  EXPECT_EQ("", aboutInfo.getCustodianExtension());
}

TEST(MainWindowMediator, DisplayWarningMessage) {
  MainWindowTestDouble mainWindow;
  MainWindowMediator mediator(&mainWindow);

  mainWindow.DisplayWarningMessage("message title", "message text");

  EXPECT_EQ(true, mainWindow.DisplayWarningMessageCalled);
  EXPECT_EQ("message title", mainWindow.DisplayWarningMessageLastTitle);
  EXPECT_EQ("message text", mainWindow.DisplayWarningMessageLastText);
}

TEST(MainWindowMediator, TreeViewColumnHeadersInitiallyHidden) {
  MainWindowTestDouble mainWindow;
  MainWindowMediator mediator(&mainWindow);

  EXPECT_EQ(true, mainWindow.HideTreeViewColumnHeadersCalled);
}
