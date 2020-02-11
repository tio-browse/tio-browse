//
// src/TIOQuantGroup.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOQuantGroup.h"
#include <typhonio.h>
#include <memory>
#include <string>
#include "src/TIOException.h"
#include "src/TIOQuant.h"

TIOQuantGroup::TIOQuantGroup(const std::string& itemName, TIOTreeItem* parent)
    : TIOTreeItem(itemName, parent) {}

TIOQuantGroup::~TIOQuantGroup() { m_childItems.clear(); }

void TIOQuantGroup::fetchMore() {
  TIOTreeItem::fetchMore();

  // Add quant items to tree structure
  TIO_Size_t nQuants;
  TIO_t tErr;
  TIO_File_t fileID = getFileID();
  TIO_Object_t meshID = m_parentItem->getObjectID();
  tErr = TIO_List_Quants(fileID, meshID, &nQuants, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
    return;
  }
  if (nQuants > 0) {
    auto quantNames =
        std::unique_ptr<char[][TIO_STRLEN]>(new char[nQuants][TIO_STRLEN]);
    tErr = TIO_List_Quants(fileID, meshID, nullptr, quantNames.get());
    if (tErr != TIO_SUCCESS) {
      throw TIOException(tErr);
    }
    for (TIO_Size_t i = 0; i < nQuants; ++i) {
      m_childItems.push_back(
          std::unique_ptr<TIOQuant>(new TIOQuant(quantNames[i], this)));
    }
  }
}

TIO_Object_t TIOQuantGroup::getObjectID() {
  return m_parentItem->getObjectID();
}
