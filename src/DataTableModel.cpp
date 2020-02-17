//
// src/DataTableModel.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "DataTableModel.h"

#include <QColor>

DataTableModel::DataTableModel(QObject *parent)
    : m_dataArray(nullptr), m_headerOrigin(0) {}

int DataTableModel::columnCount(const QModelIndex &index) const {
  return m_dataArray->getDim(0);
}

QVariant DataTableModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    int dataIndex = index.column() + index.row() * columnCount();
    return m_dataArray->toQVariant(dataIndex);
  } else if (role == Qt::BackgroundColorRole) {
    // striped rows
    if (index.row() % 2) {
      // pale gray
      return QColor(200, 200, 200, 255);
    } else {
      // white
      return QColor(255, 255, 255, 255);
    }
  } else {
    return QVariant();
  }
}

QVariant DataTableModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
  switch (role) {
    case Qt::DisplayRole:
      return section + m_headerOrigin;
      break;
    default:
      return QVariant();
  }
}

int DataTableModel::rowCount(const QModelIndex &index) const {
  // Return 1 row for 1D data
  // Return y rows for 2D data
  // Return y * z rows for 3D data
  // etc.
  int rowCount = 1;
  for (int i = 1; i < m_dataArray->getNDims(); ++i) {
    rowCount *= m_dataArray->getDim(i);
  }
  return rowCount;
}

void DataTableModel::setDataArray(DataArray *dataArray) {
  m_dataArray = dataArray;
}

void DataTableModel::setHeaderOrigin(const int &origin) {
  m_headerOrigin = origin;
  // Ensure that the table view updates the headers
  layoutAboutToBeChanged();
  layoutChanged();
}
