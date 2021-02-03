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

#include "ConvTIOtoPyTypes.h"
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
  ui->lineEdit->setText(ConvTIOtoPyTypes::NametoPy(
      QString::fromStdString(m_dataArray->getName())));
  QObject::connect(this->ui->pushButton_2, SIGNAL(clicked()), this,
                   SLOT(dialogAccepted()), Qt::QueuedConnection);
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

void DataDialog::dialogAccepted() {
  ConvTIOtoPyTypes conv;
  QString name = ui->lineEdit->text();
  QList<int> data;
  for (int i = 0; i < m_dataArray->getNDims(); ++i) {
    data.append(m_dataArray->getDim(i));
  }
  // Add an override check? 
  // Store list of names and warn if override going to happen?
  Q_EMIT addDataToConsole(conv.NametoPy(name), m_dataArray->getNDims(), data,
                          conv.TIOtoPyType(m_dataArray->getTIODataType()),
                          m_dataArray->getVoidPointer());
}
