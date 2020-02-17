//
// src/TIOTreeItem.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIOTreeItem.h"

#include <typhonio.h>

#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "TIODataItem.h"
#include "TIOException.h"
#include "TIOTypeNames.h"
#include "TIOVargroup.h"
#include "TIOVariable.h"

TIOTreeItem::TIOTreeItem(const std::string &itemName, TIOTreeItem *parent)
    : m_parentItem(parent), m_objectID(TIO_NULL), m_isInitialized(false) {
  QString itemData;
  itemData = QString().fromStdString(itemName);
  m_itemData.append(itemData);
}

TIOTreeItem::~TIOTreeItem() {}

void TIOTreeItem::AddTIODataItem(const std::string &itemName,
                                 const std::string &itemValue) {
  m_childItems.push_back(
      std::unique_ptr<TIODataItem>(new TIODataItem(itemName, itemValue, this)));
}

void TIOTreeItem::AddTIOVargroups(const TIO_File_t &fileID,
                                  const TIO_Object_t &objectID) {
  TIO_Size_t nVargroups;
  TIO_t tErr = TIO_List_Vargroups(fileID, objectID, &nVargroups, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }
  auto vargroupNames =
      std::unique_ptr<char[][TIO_STRLEN]>(new char[nVargroups][TIO_STRLEN]);
  tErr = TIO_List_Vargroups(fileID, objectID, nullptr, vargroupNames.get());
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }
  for (TIO_Size_t i = 0; i < nVargroups; ++i) {
    m_childItems.push_back(
        std::unique_ptr<TIOVargroup>(new TIOVargroup(vargroupNames[i], this)));
  }
}

void TIOTreeItem::AddTIOVariables(const TIO_File_t &fileID,
                                  const TIO_Object_t &objectID) {
  TIO_Size_t nVariables;
  TIO_t tErr = TIO_List_Variables(fileID, objectID, &nVariables, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }
  auto variableNames =
      std::unique_ptr<char[][TIO_STRLEN]>(new char[nVariables][TIO_STRLEN]);
  tErr = TIO_List_Variables(fileID, objectID, nullptr, variableNames.get());
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }
  for (TIO_Size_t i = 0; i < nVariables; ++i) {
    m_childItems.push_back(
        std::unique_ptr<TIOVariable>(new TIOVariable(variableNames[i], this)));
  }
}

bool TIOTreeItem::canFetchMore() {
  // Assume that if the node is not initialized it may have some children
  if (m_isInitialized == false) {
    return true;
  } else {
    return false;
  }
}

TIOTreeItem *TIOTreeItem::child(int row) {
  if (row < m_childItems.size()) {
    return m_childItems.at(row).get();
  }
  return nullptr;
}

int TIOTreeItem::childCount() const { return m_childItems.size(); }

int TIOTreeItem::columnCount() const { return m_itemData.count(); }

QVariant TIOTreeItem::data(int column) const {
  return m_itemData.value(column);
}

void TIOTreeItem::fetchMore() { m_isInitialized = true; }

DataArray *TIOTreeItem::getArrayData() { return nullptr; }

DataArray *TIOTreeItem::getArrayData(const std::string &variable) {
  return nullptr;
}

TIO_File_t TIOTreeItem::getFileID() { return m_parentItem->getFileID(); }

TIO_Object_t TIOTreeItem::getObjectID() { return m_objectID; }

TIO_Object_t TIOTreeItem::getMeshID() { return m_parentItem->getMeshID(); }

TIOTreeItem *TIOTreeItem::getParentItem() { return m_parentItem; }

bool TIOTreeItem::hasArrayData() {
  // Only TIODataItem has array data
  return false;
}

bool TIOTreeItem::hasChildren() {
  // All items have children, except for TIOInfoItem
  return true;
}

TIOTreeItem *TIOTreeItem::parent() { return m_parentItem; }

int TIOTreeItem::row() const {
  if (m_parentItem) {
    for (unsigned int i = 0; i < m_parentItem->m_childItems.size(); ++i) {
      if (m_parentItem->m_childItems[i].get() == this) return i;
    }
  }

  return 0;
}
