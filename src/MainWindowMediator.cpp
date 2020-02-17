//
// src/MainWindowMediator.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "MainWindowMediator.h"

#include <typhonio.h>

#include <string>

MainWindowMediator::MainWindowMediator(MainWindowMediatorInterface *window) {
  aboutInfo.setAppName("TIO browse");
  aboutInfo.setAppVersion("0.0.1");
  aboutInfo.setAppDescription(
      "TIO browse is a viewer for exploring the "
      "structure of TyphonIO files");
  aboutInfo.setCustodianName("Jim Eliot");
  aboutInfo.setCustodianEmail("jim.eliot@awe.co.uk");
  aboutInfo.setCustodianExtension("");

  this->window = window;
  window->SetMediator(this);

  window->HideTreeViewColumnHeaders();
}

MainWindowMediator::~MainWindowMediator() {}

const AboutInfo &MainWindowMediator::getAboutInfo() const { return aboutInfo; }

void MainWindowMediator::Exit() { window->CloseDialog(); }

void MainWindowMediator::OpenFile(const std::string &fileName) {
  TIO_t tioError;

  this->fileName = fileName;

  tioError = TIO_CheckFile(this->fileName.c_str());
  if (tioError != TIO_SUCCESS) {
    window->DisplayWarningMessage(
        "Invalid file",
        fileName + " was not recognized as a valid TyphonIO file");
    return;
  }

  // Attempt to open TyphonIO file and read data
  try {
    window->InitializeTreeModel(fileName);
  } catch (std::exception &e) {
    window->DisplayWarningMessage(
        "TyphonIO error",
        "An error occurred when reading from file " + fileName);
  }
}
