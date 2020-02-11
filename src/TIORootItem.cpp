//
// src/TIORootItem.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//
//

#include <src/TIORootItem.h>
#include <memory>
#include <string>
#include "src/TIOFile.h"

TIORootItem::TIORootItem(const std::string &itemName, TIOTreeItem *parent)
    : TIOTreeItem::TIOTreeItem(itemName, parent) {}

TIORootItem::~TIORootItem() { m_childItems.clear(); }

void TIORootItem::OpenFile(const std::string &fileName) {
  m_childItems.push_back(
      std::unique_ptr<TIOFile>(new TIOFile(fileName, fileName, this)));
}
