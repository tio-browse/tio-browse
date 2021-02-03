//
// src/MainWindow.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_MAINWINDOW_H_
#define SRC_MAINWINDOW_H_

#ifdef CONSOLE
#include "qpyconsole.h"
#include <QtWidgets>
#else
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#endif

#include <string>

#include "MainWindowMediator.h"
#include "TIOTreeModel.h"

namespace Ui {
class MainWindow;
}

//! Main window for the app
//!
//! Main window that is displayed when the app is started
//!
class MainWindow : public QMainWindow, public MainWindowMediatorInterface {
  Q_OBJECT

 public:
  //! Constructor
  //!
  //! Constructor for MainWIndow
  //!
  //! \param[in] parent is the parent object, default is zero (none)
  explicit MainWindow(QWidget *parent = 0);

  ~MainWindow() override;

  //! Set the dialog mediator
  //!
  //! \param[in] mediator is a pointer to a dialog mediator
  void SetMediator(MainWindowMediator *mediator) override;

  //! Close the dialog
  void CloseDialog() override;

  //! Request that a warning message is displayed
  //!
  //! \param[in] messageTitle is the message title text
  //! \param[in] messageText is the message main text
  //!
  void DisplayWarningMessage(const std::string &messageTitle,
                             const std::string &messageText) override;

  //! Request that the tree view column headers are hidden
  //!
  void HideTreeViewColumnHeaders() override;

  //! Request that the tree model is initialized
  //!
  void InitializeTreeModel(std::string filename) override;

 public slots:
  //! Open menu item triggered slot
  //!
  //! Request a open file selection dialog
  //!
  void on_action_Open_triggered();

  //! Close menu item triggered slot
  //!
  //! Request that the current file is closed
  //!
  void on_action_Close_triggered();

  //! Exit menu item triggered slot
  //!
  //! Exits the app
  //!
  void on_action_Exit_triggered();

  //! Display help menu item triggered slot
  //!
  //! Opens the documentation in the default browser
  //!
  void on_action_Display_Help_triggered();

  //! About menu item triggered slot
  //!
  //! Displays the About dialog
  //!
  void on_action_About_TIO_browse_triggered();

  //! Tree view node expanded slot
  //!
  //! Called by the main tree view widget when a node of the tree
  //! is expanded.
  //!
  void on_treeView_expanded(QModelIndex index);

  //! Tree view activated slot
  //! Called my the main tree view widget when a node of the tree is activated
  void on_treeView_activated(QModelIndex index);

 private:
  Ui::MainWindow *ui;
  MainWindowMediator *mediator;
  TIOTreeModel *m_tioTreeModel;
#ifdef CONSOLE
  void createMenus();
  QMenu *consoleMenu;
  QAction *showConsoleAct;
  void createDockWindows();
  QDockWidget *dock;
  QPyConsole *console;
#endif
};

#endif  // SRC_MAINWINDOW_H_
