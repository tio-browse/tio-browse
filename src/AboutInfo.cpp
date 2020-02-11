//
// src/AboutInfo.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/AboutInfo.h"
#include <string>

AboutInfo::AboutInfo() {}

const std::string& AboutInfo::getAppDescription() const {
  return appDescription;
}

void AboutInfo::setAppDescription(const std::string& appDescription) {
  this->appDescription = appDescription;
}

const std::string& AboutInfo::getAppName() const { return appName; }

void AboutInfo::setAppName(const std::string& appName) {
  this->appName = appName;
}

const std::string& AboutInfo::getAppVersion() const { return appVersion; }

void AboutInfo::setAppVersion(const std::string& appVersion) {
  this->appVersion = appVersion;
}

const std::string& AboutInfo::getCustodianEmail() const {
  return custodianEmail;
}

void AboutInfo::setCustodianEmail(const std::string& custodianEmail) {
  this->custodianEmail = custodianEmail;
}

const std::string& AboutInfo::getCustodianExtension() const {
  return custodianExtension;
}

void AboutInfo::setCustodianExtension(const std::string& custodianExtension) {
  this->custodianExtension = custodianExtension;
}

const std::string& AboutInfo::getCustodianName() const { return custodianName; }

void AboutInfo::setCustodianName(const std::string& custodianName) {
  this->custodianName = custodianName;
}
