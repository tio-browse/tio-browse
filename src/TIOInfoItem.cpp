//
// src/TIOInfoItem.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOInfoItem.h"
#include <string>

TIOInfoItem::TIOInfoItem(const std::string& itemName,
                         const std::string& itemValue, TIOTreeItem* parent)
    : TIOTreeItem(itemName, parent) {
  QString valueData;
  valueData = QString().fromStdString(itemValue);
  m_itemData.append(valueData);
}

TIOInfoItem::~TIOInfoItem() { m_childItems.clear(); }

QVariant TIOInfoItem::data(int column) const {
  if (m_itemData.count() > column + 1) {
    return m_itemData.value(column).toString() + QString("  :  ") +
           m_itemData.value(column + 1).toString();
  } else {
    return m_itemData.value(column);
  }
}

bool TIOInfoItem::canFetchMore() { return false; }

bool TIOInfoItem::hasChildren() { return false; }
