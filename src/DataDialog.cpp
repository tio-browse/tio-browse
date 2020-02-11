//
// src/DataDialog.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/DataDialog.h"
#include <QAbstractItemModel>
#include "src/DataTableModel.h"

DataDialog::DataDialog(DataArray* dataArray, QWidget* parent)
    : QDialog(parent), ui(new Ui::DataDialog), m_dataArray(dataArray) {
  ui->setupUi(this);
  ui->oneOriginCheckBox->setCheckState(Qt::Unchecked);
  m_dataTableModel.setDataArray(m_dataArray);
  ui->tableView->setModel(static_cast<QAbstractItemModel*>(&m_dataTableModel));
}

DataDialog::~DataDialog() {
  if (m_dataArray != nullptr) {
    delete m_dataArray;
  }
  delete ui;
}

void DataDialog::on_oneOriginCheckBox_stateChanged(int state) {
  switch (state) {
    case Qt::Unchecked:
      m_dataTableModel.setHeaderOrigin(0);
      break;
    case Qt::Checked:
      m_dataTableModel.setHeaderOrigin(1);
      break;
    default:
      m_dataTableModel.setHeaderOrigin(0);
  }
}
