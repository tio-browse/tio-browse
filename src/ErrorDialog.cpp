//
// src/ErrorDialog.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//
//

#include "src/ErrorDialog.h"
#include <string>

void ErrorDialog(const std::string &message) {
  QMessageBox::warning(nullptr, "Error", message.c_str(), QMessageBox::Ok);
}

void ErrorDialog(const std::string &functionName, const std::string &message) {
  QString text;
  text.append("An error occurred in");
  text.append("\n");
  text.append(functionName.c_str());
  text.append("\n");
  text.append(message.c_str());
  QMessageBox::warning(nullptr, "Error", text, QMessageBox::Ok);
}
