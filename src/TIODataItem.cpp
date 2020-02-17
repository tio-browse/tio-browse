//
// src/TIODataItem.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIODataItem.h"

#include <string>

TIODataItem::TIODataItem(const std::string &itemName,
                         const std::string &itemValue, TIOTreeItem *parent)
    : TIOTreeItem(itemName, parent) {
  QString itemData = QString().fromStdString(itemValue);
  m_itemData.append(itemData);
}

TIODataItem::~TIODataItem() { m_childItems.clear(); }

bool TIODataItem::canFetchMore() { return false; }

QVariant TIODataItem::data(int column) const {
  if (m_itemData.count() > column + 1) {
    return m_itemData.value(column).toString() + QString("  :  ") +
           m_itemData.value(column + 1).toString();
  } else {
    return m_itemData.value(column);
  }
}

DataArray *TIODataItem::getArrayData() {
  return m_parentItem->getArrayData(
      m_itemData.value(0).toString().toStdString());
}

bool TIODataItem::hasArrayData() { return true; }

bool TIODataItem::hasChildren() { return false; }
