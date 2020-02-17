//
// src/MainWindowMediator.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_MAINWINDOWMEDIATOR_H_
#define SRC_MAINWINDOWMEDIATOR_H_

#include <string>

#include "AboutInfo.h"

class MainWindowMediator;

//! Interface to MainWindow used by MainWindowMediator
//!
//! This abstract class is implemented by a MainWindow to
//! allow the dialog to interact with the dialog without
//! knowledge of its implementation
//!
class MainWindowMediatorInterface {
 public:
  //! Constructor
  //!
  virtual ~MainWindowMediatorInterface() {}

  //! Implemented by the child class to set the mediator
  //!
  virtual void SetMediator(MainWindowMediator *mediator) = 0;

  //! Close the dialog
  //!
  //! Implemented by the child class to allow the mediator to close the dialog
  //!
  virtual void CloseDialog() = 0;

  //! Request that a warning message is displayed
  //!
  //! Implemented by the child class to allow the mediator to request a
  //! warning message be displayed
  //!
  //! \param[in] messageTitle is the message title text
  //! \param[in] messageText is the message main text
  //!
  virtual void DisplayWarningMessage(const std::string &messageTitle,
                                     const std::string &messageText) = 0;

  //! Request that the column headers in the tree view are hidden
  //!
  //! Implemented by the child class to allow the mediator to request that
  //! tree view headers are hidden
  //!
  virtual void HideTreeViewColumnHeaders() = 0;

  //! Intialize the tree model
  //!
  //! Implemented by the child class to allow the mediator to request that
  //! the tre model is initialized
  //!
  virtual void InitializeTreeModel(std::string filename) = 0;
};

//! Mediator class for MainWindow
//!
class MainWindowMediator {
 public:
  //! Constructor
  //!
  //! \param[in] window is a pointer to a MainWindow dialog
  //!
  explicit MainWindowMediator(MainWindowMediatorInterface *window);

  ~MainWindowMediator();

  //! Returns information about the app
  //!
  //! \return AboutInfo instance populated with information about the app
  //!
  const AboutInfo &getAboutInfo() const;

  //! Exit the app
  //!
  void Exit();

  //! Open file
  //!
  //! \param[in] fileName is the name of the file to open
  //!
  void OpenFile(const std::string &fileName);

 private:
  MainWindowMediatorInterface *window;
  AboutInfo aboutInfo;
  std::string fileName;
};

#endif  // SRC_MAINWINDOWMEDIATOR_H_
