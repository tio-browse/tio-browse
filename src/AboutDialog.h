//
// src/AboutDialog.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_ABOUTDIALOG_H_
#define SRC_ABOUTDIALOG_H_

#include "src/ui_AboutDialog.h"
#include "src/AboutInfo.h"

namespace Ui {
class AboutDialog;
}

//! About dialog
//!
//! Dialog for displaying information about the app
//!
class AboutDialog : public QDialog {
  Q_OBJECT
 public:
  //! Constructor
  //!
  //! \param[in] parent is the parent object, default is zero (none)
  //!
  explicit AboutDialog(QWidget* parent = 0);

  //! Destructor
  //!
  ~AboutDialog() override;

  //! Setter for the about dialog information
  //!
  void SetAboutInfo(const AboutInfo& aboutInfo);

 private:
  Ui::AboutDialog* ui;
};

#endif  // SRC_ABOUTDIALOG_H_
