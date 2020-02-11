//
// src/TIOInterfaceGroup.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOInterfaceGroup.h"
#include <memory>
#include <string>
#include "src/TIOException.h"
#include "src/TIOInterface.h"

TIOInterfaceGroup::TIOInterfaceGroup(const std::string& itemName,
                                     TIOTreeItem* parent)
    : TIOTreeItem(itemName, parent) {}

TIOInterfaceGroup::~TIOInterfaceGroup() { m_childItems.clear(); }

void TIOInterfaceGroup::fetchMore() {
  TIOTreeItem::fetchMore();

  // Add interface items to tree structure
  TIO_Size_t nInterfaces;
  TIO_t tErr;
  TIO_File_t fileID = getFileID();
  TIO_Object_t stateID = m_parentItem->getObjectID();
  tErr = TIO_List_Interfaces(fileID, stateID, &nInterfaces, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
    return;
  }
  if (nInterfaces > 0) {
    auto interfaceNames =
        std::unique_ptr<char[][TIO_STRLEN]>(new char[nInterfaces][TIO_STRLEN]);
    tErr = TIO_List_Interfaces(fileID, stateID, nullptr, interfaceNames.get());
    if (tErr != TIO_SUCCESS) {
      throw TIOException(tErr);
    }
    for (TIO_Size_t i = 0; i < nInterfaces; ++i) {
      m_childItems.push_back(std::unique_ptr<TIOInterface>(
          new TIOInterface(interfaceNames[i], this)));
    }
  }
}

TIO_Object_t TIOInterfaceGroup::getObjectID() {
  return m_parentItem->getObjectID();
}
