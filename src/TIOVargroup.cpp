//
// src/TIOVargroup.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIOVargroup.h"

#include <string>

#include "TIOException.h"

TIOVargroup::TIOVargroup(const std::string &itemName, TIOTreeItem *parent)
    : TIOTreeItem(itemName, parent) {}

TIOVargroup::~TIOVargroup() {
  m_childItems.clear();

  TIO_File_t fileID = m_parentItem->getFileID();
  if (fileID != TIO_NULL && m_objectID != TIO_NULL) {
    TIO_Close_Vargroup(fileID, m_objectID);
  }
}

void TIOVargroup::fetchMore() {
  TIOTreeItem::fetchMore();

  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t objectID = m_parentItem->getObjectID();

  TIO_t tErr = TIO_Open_Vargroup(fileID, objectID,
                                 m_itemData[0].toString().toStdString().c_str(),
                                 &m_objectID);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Add variables and vargroups
  AddTIOVariables(fileID, m_objectID);
  AddTIOVargroups(fileID, m_objectID);
}
