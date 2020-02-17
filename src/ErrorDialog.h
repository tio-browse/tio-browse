//
// src/ErrorDialog.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//
//

#ifndef SRC_ERRORDIALOG_H_
#define SRC_ERRORDIALOG_H_

#include <QMessageBox>
#include <string>

void ErrorDialog(const std::string &message);

void ErrorDialog(const std::string &functionName, const std::string &message);

#endif  // SRC_ERRORDIALOG_H_
