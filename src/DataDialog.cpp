//
// src/DataDialog.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "DataDialog.h"

#include <QAbstractItemModel>
#include <QDebug>
#include "DataTableModel.h"
#include "ui_DataDialog.h"

DataDialog::DataDialog(DataArray *dataArray, QWidget *parent)
    : QDialog(parent), ui(new Ui::DataDialog), m_dataArray(dataArray) {
  ui->setupUi(this);
  ui->oneOriginCheckBox->setCheckState(Qt::Unchecked);
  #ifndef CONSOLE
  ui->pushButton->hide();
  #endif
  ui->groupBox->hide();
  m_dataTableModel.setDataArray(m_dataArray);
  ui->tableView->setModel(static_cast<QAbstractItemModel *>(&m_dataTableModel));
  ui->lineEdit->setText(QString::fromStdString(m_dataArray->getName()));
  QObject::connect(this, SIGNAL(accepted()), this, SLOT(dialogAccepted()), Qt::QueuedConnection);
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

void DataDialog::dialogAccepted(){
  QString name = ui->lineEdit->text();
  // qInfo() << name ;
  QList<int> data;
  for (int i = 0; i < m_dataArray->getNDims(); ++i) {
    data.append(m_dataArray->getDim(i));
  }
  qInfo() << data;
  // data[0] = 1;
  // data[1] = 69;
  // void * arr =data;
  // dataArray->QString name, const int ND, int* DIMS , int TypeInt, void* data
  Q_EMIT addDataToConsole(name, m_dataArray->getNDims(),data,12,m_dataArray->getVoidPointer());

  // int x = ((int *)arr)[1];
  qInfo() << m_dataArray->getVoidPointer();
}


 