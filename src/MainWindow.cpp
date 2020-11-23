//
// src/MainWindow.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "MainWindow.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QUrl>
#include <string>

#include "AboutDialog.h"
#include "DataArray.h"
#include "DataDialog.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mediator(nullptr),
      m_tioTreeModel(nullptr) {
  ui->setupUi(this);
  ui->treeView->setModel(nullptr);
  #ifdef CONSOLE
    this->createDockWindows();   
  #endif
}

MainWindow::~MainWindow() {
  if (m_tioTreeModel != nullptr) {
    delete m_tioTreeModel;
  }
}

void MainWindow::SetMediator(MainWindowMediator *mediator) {
  this->mediator = mediator;
}

void MainWindow::CloseDialog() { this->close(); }

void MainWindow::DisplayWarningMessage(const std::string &messageTitle,
                                       const std::string &messageText) {
  QMessageBox::warning(this, QString::fromStdString(messageTitle),
                       QString::fromStdString(messageText), QMessageBox::Ok, 0);
}

void MainWindow::HideTreeViewColumnHeaders() { ui->treeView->header()->hide(); }

void MainWindow::InitializeTreeModel(std::string filename) {
  m_tioTreeModel = new TIOTreeModel(filename, this);
  ui->treeView->setModel(m_tioTreeModel);
}

void MainWindow::on_action_Open_triggered() {
  // Present Open File dialog
  QString selected = tr("TyphonIO (*.h5)");
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open File"), "", tr("TyphonIO (*.h5);;All Files (*)"),
      &selected);

  // fileName is null if file dialog was cancelled
  if (!fileName.isNull()) {
    // Return the file name as std::string, using UTF-8 encoding to support
    // non-latin characters
    std::string fileName_utf8 = fileName.toUtf8().constData();
    mediator->OpenFile(fileName_utf8);
  }
}

void MainWindow::on_action_Close_triggered() {}

void MainWindow::on_action_Exit_triggered() { mediator->Exit(); }

void MainWindow::on_action_Display_Help_triggered() {
  QDesktopServices::openUrl(
      QUrl(QApplication::applicationDirPath() + "/../docs/html/index.html",
           QUrl::TolerantMode));
}

void MainWindow::on_action_About_TIO_browse_triggered() {
  AboutDialog aboutDialog;
  aboutDialog.SetAboutInfo(mediator->getAboutInfo());
  aboutDialog.exec();  // modal
}

void MainWindow::on_treeView_expanded(QModelIndex index) {}

void MainWindow::on_treeView_activated(QModelIndex index) {
  if (m_tioTreeModel->hasArrayData(index)) {
    DataArray *dataArray = m_tioTreeModel->getArrayData(index);
    if (dataArray != nullptr) {
      DataDialog *dataDialog = new DataDialog(dataArray, this);
      dataDialog->show();
      dataDialog->exec();
    }
  }
}

/**
 * @brief Create all the Dock Windows
 * 
 */
void MainWindow::createDockWindows(){
    QDockWidget* dock = new QDockWidget(tr("Python Console"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    
    QWidget *window = new QWidget;
    window->setMinimumSize(320, 240);
    QPyConsole* console = QPyConsole::getInstance(window,"Type \"help()\" for more information:");
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    window->setLayout(layout);
    layout->addWidget(console);
    dock->setWidget(window);
    this->addDockWidget(Qt::BottomDockWidgetArea, dock);

}
