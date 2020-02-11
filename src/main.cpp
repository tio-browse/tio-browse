//
// src/main.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/MainWindow.h"
#include "src/MainWindowMediator.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  MainWindow mainWindow;
  MainWindowMediator mediator(&mainWindow);
  mainWindow.show();

  return app.exec();
}
