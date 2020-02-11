//
// src/AboutDialog.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/AboutDialog.h"

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::AboutDialog) {
  ui->setupUi(this);
}

AboutDialog::~AboutDialog() { delete ui; }

void AboutDialog::SetAboutInfo(const AboutInfo& aboutInfo) {
  QString appNameText = QString::fromStdString(aboutInfo.getAppName());
  QString appVersionText = QString::fromStdString(aboutInfo.getAppVersion());
  QString appDescriptionText =
      QString::fromStdString(aboutInfo.getAppDescription());
  QString custodianName = QString::fromStdString(aboutInfo.getCustodianName());
  QString custodianExtension =
      QString::fromStdString(aboutInfo.getCustodianExtension());
  QString custodianEmail =
      QString::fromStdString(aboutInfo.getCustodianEmail());
  ui->appNameLabel->setText(appNameText);
  ui->appVersionLabel->setText(appVersionText);
  ui->appDescriptionLabel->setText(appDescriptionText);
  ui->custodianNameLabel->setText(custodianName);
  ui->custodianExtensionLabel->setText(custodianExtension);
  ui->custodianEmailLabel->setText(custodianEmail);

  QString windowTitle = "About " + appNameText;
  this->setWindowTitle(windowTitle);
}
