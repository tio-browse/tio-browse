//
// test/MainWindowTestDouble.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "MainWindowTestDouble.h"

#include <string>

MainWindowTestDouble::MainWindowTestDouble()
    : CloseDialogCalled(false),
      DisplayWarningMessageCalled(false),
      mediator(nullptr),
      HideTreeViewColumnHeadersCalled(false),
      InitializeTreeModelCalled(false) {}

void MainWindowTestDouble::SetMediator(MainWindowMediator *mediator) {
  this->mediator = mediator;
}

void MainWindowTestDouble::CloseDialog() { CloseDialogCalled = true; }

void MainWindowTestDouble::DisplayWarningMessage(
    const std::string &messageTitle, const std::string &messageText) {
  DisplayWarningMessageCalled = true;
  DisplayWarningMessageLastTitle = messageTitle;
  DisplayWarningMessageLastText = messageText;
}

void MainWindowTestDouble::HideTreeViewColumnHeaders() {
  HideTreeViewColumnHeadersCalled = true;
}

void MainWindowTestDouble::InitializeTreeModel(std::string filename) {
  InitializeTreeModelCalled = true;
  InitializeTreeModelLastFilename = filename;
}
