//
// src/DataDialog.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_DATADIALOG_H_
#define SRC_DATADIALOG_H_

#include <QtWidgets/QDialog>
#include <string>
#include "DataArray.h"
#include "DataTableModel.h"

namespace Ui {
class DataDialog;
}

//! Data dialog
//!
//! Dialog for displaying data as a table
//!
class DataDialog : public QDialog {
  Q_OBJECT
 public:
  //! Constructor
  //! \param[in] dataArray is a pointer to the data instance
  //! \param[in] parent is the parent object, default is zero (none)
  explicit DataDialog(DataArray *dataArray, QWidget *parent = 0);

  //! Destructor
  ~DataDialog() override;

  #ifdef CONSOLE
  Q_SIGNALS:
    void addDataToConsole(QString name, const int ND, QList<int> DIMS , int TypeInt, void* data);//QString name, DataArray *dataArray);
  public Q_SLOTS:
    void dialogAccepted();
  #else 
  public slots:
  #endif

  //! One-origin check box state changed
  void on_oneOriginCheckBox_stateChanged(int state);

 private:
  Ui::DataDialog *ui;
  DataArray *m_dataArray;
  DataTableModel m_dataTableModel;
  DataDialog(const DataDialog &) = delete;
};

#endif  // SRC_DATADIALOG_H_
