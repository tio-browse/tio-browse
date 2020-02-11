//
// src/TIOInfoGroup.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOInfoGroup.h"
#include <memory>
#include <string>
#include "src/TIOInfoItem.h"

TIOInfoGroup::TIOInfoGroup(const std::string& itemName, TIOTreeItem* parent)
    : TIOTreeItem(itemName, parent) {}

TIOInfoGroup::~TIOInfoGroup() { m_childItems.clear(); }

void TIOInfoGroup::AddTIOInfoItem(std::string itemName, std::string itemValue) {
  m_childItems.push_back(
      std::unique_ptr<TIOInfoItem>(new TIOInfoItem(itemName, itemValue, this)));
}

DataArray* TIOInfoGroup::getArrayData(const std::string& variable) {
  return m_parentItem->getArrayData(variable);
}
