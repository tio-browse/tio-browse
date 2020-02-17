//
// src/TIOTreeModel.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIOTreeModel.h"

#include <string>

#include "ErrorDialog.h"
#include "TIOException.h"
#include "TIORootItem.h"

TIOTreeModel::TIOTreeModel(const std::string &filename, QObject *parent)
    : m_filename(filename) {
  m_rootItem = std::unique_ptr<TIORootItem>(new TIORootItem("root item"));
  m_rootItem->OpenFile(m_filename);
}

TIOTreeModel::~TIOTreeModel() {}

bool TIOTreeModel::canFetchMore(const QModelIndex &parent) const {
  TIOTreeItem *parentItem;

  if (!parent.isValid())
    parentItem = m_rootItem.get();
  else
    parentItem = static_cast<TIOTreeItem *>(parent.internalPointer());

  return parentItem->canFetchMore();
}

int TIOTreeModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return static_cast<TIOTreeItem *>(parent.internalPointer())->columnCount();
  else
    return m_rootItem->columnCount();
}

QVariant TIOTreeModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return QVariant();

  switch (role) {
    case Qt::DisplayRole:
      break;
    case Qt::FontRole:
      return QVariant();
      break;
    default:
      return QVariant();
  }

  TIOTreeItem *item = static_cast<TIOTreeItem *>(index.internalPointer());
  try {
    return item->data(index.column());
  } catch (TIOException &e) {
    ErrorDialog("TIOTreeModel::fetchMore",
                "TyphonIO exception: " + std::string(e.what()));
  }
  return QVariant();
}

// TODO(jim): Modify getArrayData to return DataArray instead of DataArray *
DataArray *TIOTreeModel::getArrayData(const QModelIndex &index) const {
  TIOTreeItem *item;

  if (!index.isValid())
    item = m_rootItem.get();
  else
    item = static_cast<TIOTreeItem *>(index.internalPointer());

  try {
    return item->getArrayData();
  } catch (TIOException &e) {
    ErrorDialog("TIOTreeModel::fetchMore",
                "TyphonIO exception: " + std::string(e.what()));
  } catch (TIOMeshtypeException &e) {
    ErrorDialog("TIOTreeModel::fetchMore",
                "TyphonIO exception: " + std::string(e.what()));
  } catch (TIOVariableNameException &e) {
    ErrorDialog("TIOTreeModel::fetchMore",
                "TyphonIO exception: " + std::string(e.what()));
  }
  return nullptr;
}

bool TIOTreeModel::hasArrayData(const QModelIndex &index) const {
  TIOTreeItem *item;

  if (!index.isValid())
    item = m_rootItem.get();
  else
    item = static_cast<TIOTreeItem *>(index.internalPointer());

  return item->hasArrayData();
}

void TIOTreeModel::fetchMore(const QModelIndex &parent) {
  TIOTreeItem *parentItem;

  if (!parent.isValid())
    parentItem = m_rootItem.get();
  else
    parentItem = static_cast<TIOTreeItem *>(parent.internalPointer());

  try {
    parentItem->fetchMore();
  } catch (TIOException &e) {
    ErrorDialog("TIOTreeModel::fetchMore",
                "TyphonIO exception: " + std::string(e.what()));
  }
}

bool TIOTreeModel::hasChildren(const QModelIndex &parent) const {
  TIOTreeItem *parentItem;

  if (!parent.isValid())
    parentItem = m_rootItem.get();
  else
    parentItem = static_cast<TIOTreeItem *>(parent.internalPointer());

  return parentItem->hasChildren();
}

QModelIndex TIOTreeModel::index(int row, int column,
                                const QModelIndex &parent) const {
  TIOTreeItem *parentItem;

  if (!parent.isValid())
    parentItem = m_rootItem.get();
  else
    parentItem = static_cast<TIOTreeItem *>(parent.internalPointer());

  TIOTreeItem *childItem = parentItem->child(row);
  if (childItem) return createIndex(row, column, childItem);

  return QModelIndex();
}

QModelIndex TIOTreeModel::parent(const QModelIndex &index) const {
  if (!index.isValid()) return QModelIndex();

  TIOTreeItem *childItem = static_cast<TIOTreeItem *>(index.internalPointer());
  TIOTreeItem *parentItem = childItem->parent();

  if (parentItem == m_rootItem.get()) return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int TIOTreeModel::rowCount(const QModelIndex &parent) const {
  TIOTreeItem *parentItem;

  if (!parent.isValid())
    parentItem = m_rootItem.get();
  else
    parentItem = static_cast<TIOTreeItem *>(parent.internalPointer());

  return parentItem->childCount();
}
