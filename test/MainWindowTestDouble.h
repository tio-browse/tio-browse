//
// test/MainWindowTestDouble.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef TEST_MAINWINDOWTESTDOUBLE_H_
#define TEST_MAINWINDOWTESTDOUBLE_H_

#include <string>

#include "MainWindowMediator.h"

class MainWindowTestDouble : public MainWindowMediatorInterface {
 public:
  MainWindowTestDouble();
  void SetMediator(MainWindowMediator *mediator) override;
  void CloseDialog() override;
  void DisplayWarningMessage(const std::string &messageTitle,
                             const std::string &messageText) override;
  void HideTreeViewColumnHeaders() override;
  void InitializeTreeModel(std::string filename) override;

  bool CloseDialogCalled;
  bool DisplayWarningMessageCalled;
  std::string DisplayWarningMessageLastTitle;
  std::string DisplayWarningMessageLastText;
  bool HideTreeViewColumnHeadersCalled;
  bool InitializeTreeModelCalled;
  std::string InitializeTreeModelLastFilename;

 private:
  MainWindowMediator *mediator;
};

#endif  // TEST_MAINWINDOWTESTDOUBLE_H_
